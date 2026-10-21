/*
 * A099490  Least k such that the interval [n,k] contains a subset of integers
 *          whose product is twice a square.
 * https://oeis.org/A099490
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 72, u16 n <= 72, u32 n <= 72, u64 n <= 72, u128 n <= 72,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS sequence A099490
 *  a(n) = least k ≥ n such that the interval [n,k] contains a subset of
 *  integers whose product equals 2·(square).
 *  This implementation searches k by adding the parity vectors of the
 *  numbers in the interval to a GF(2) linear basis and checking
 *  whether the target vector (only the prime 2 has odd exponent) is
 *  in the span.  The algorithm runs in O(n²) in the worst case but
 *  is fast enough for n ≤ 10 000.  */

#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

/* ----- constants ---------------------------------------------------- */
#define MAX_LIMIT     200000   /* sieve limit – sufficient for n ≤ 10 000 */
#define MAX_WORDS     300      /* power‑of‑two words for prime indices */

/* ----- global tables and state ------------------------------------- */
static uint32_t *spf          = NULL;   /* smallest prime factor   */
static uint32_t *prime_idx    = NULL;   /* prime → bit index      */
static uint32_t *primes       = NULL;   /* list of primes         */
static size_t    prime_cnt    = 0;      /* number of primes       */
static size_t    word_cnt     = 0;      /* words per parity vector*/
static uint64_t  target_vec[MAX_WORDS];/* vector for prime 2      */

typedef struct { uint64_t v[MAX_WORDS]; } BasisVec;
static BasisVec *basis_vecs = NULL;     /* linear basis vectors   */
static size_t    basis_cap   = 0;       /* allocated capacity     */
static size_t    basis_len   = 0;       /* current number of vectors */
static int       *basis_pivots = NULL;  /* pivot bit of each vector*/

static seq_t  *cache      = NULL;      /* memoised a(n)          */
static size_t  cache_sz   = 0;         /* array capacity         */
static size_t  cache_len  = 0;         /* how many values cached */

/* ----- helper: initialise sieve ----------------------------------- */
static void init_sieve(void)
{
    if (spf) return;                         /* already built */

    spf   = (uint32_t*)calloc(MAX_LIMIT + 1, sizeof(uint32_t));
    primes = (uint32_t*)malloc((MAX_LIMIT / 10) * sizeof(uint32_t));
    prime_cnt = 0;

    for (uint32_t i = 2; i <= MAX_LIMIT; ++i) {
        if (!spf[i]) {
            spf[i] = i;
            primes[prime_cnt++] = i;
        }
        for (size_t j = 0; j < prime_cnt; ++j) {
            uint64_t prod = (uint64_t)i * primes[j];
            if (prod > MAX_LIMIT) break;
            spf[prod] = primes[j];
            if (primes[j] == spf[i]) break;
        }
    }

    word_cnt = (prime_cnt + 63) / 64;

    prime_idx = (uint32_t*)calloc(MAX_LIMIT + 1, sizeof(uint32_t));
    for (size_t i = 0; i < prime_cnt; ++i) prime_idx[primes[i]] = (uint32_t)i;

    memset(target_vec, 0, sizeof(target_vec));
    target_vec[0] |= 1ULL;                    /* prime 2 index 0 */

    basis_cap = 1000;
    basis_vecs = (BasisVec*)malloc(basis_cap * sizeof(BasisVec));
    basis_pivots = (int*)malloc(basis_cap * sizeof(int));
}

/* ----- vector utilities -------------------------------------------- */
static void vec_zero(uint64_t *v)
{
    memset(v, 0, word_cnt * sizeof(uint64_t));
}

static bool vec_is_zero(const uint64_t *v)
{
    for (size_t i = 0; i < word_cnt; ++i)
        if (v[i]) return false;
    return true;
}

static int vec_highest_bit(const uint64_t *v)
{
    for (int i = (int)word_cnt - 1; i >= 0; --i) {
        if (v[i]) return i * 64 + 63 - (__builtin_clzll(v[i]) & 63);
    }
    return -1;
}

/* parity vector of an integer (mod 2 exponents of prime factors) */
static void parity_vector(idx_t num, uint64_t *out)
{
    vec_zero(out);
    uint64_t x = num;
    while (x > 1) {
        uint32_t p = spf[x];
        int idx = prime_idx[p];
        out[idx >> 6] ^= 1ULL << (idx & 63);
        x /= p;
    }
}

/* insert vector into the linear basis over GF(2) */
static void basis_insert(const uint64_t *vec)
{
    uint64_t cur[MAX_WORDS];
    memcpy(cur, vec, word_cnt * sizeof(uint64_t));

    for (size_t i = 0; i < basis_len; ++i) {
        if ((cur[basis_pivots[i] >> 6] >> (basis_pivots[i] & 63)) & 1ULL) {
            for (size_t w = 0; w < word_cnt; ++w)
                cur[w] ^= basis_vecs[i].v[w];
        }
    }
    if (vec_is_zero(cur)) return;                /* already in span */

    if (basis_len == basis_cap) {
        basis_cap *= 2;
        basis_vecs = (BasisVec*)realloc(basis_vecs, basis_cap * sizeof(BasisVec));
        basis_pivots = (int*)realloc(basis_pivots, basis_cap * sizeof(int));
    }
    memcpy(basis_vecs[basis_len].v, cur, word_cnt * sizeof(uint64_t));
    basis_pivots[basis_len] = vec_highest_bit(cur);
    ++basis_len;
}

/* check if current basis contains the target vector */
static bool target_in_span(void)
{
    static uint64_t tmp[MAX_WORDS];
    memcpy(tmp, target_vec, word_cnt * sizeof(uint64_t));

    for (size_t i = 0; i < basis_len; ++i) {
        if ((tmp[basis_pivots[i] >> 6] >> (basis_pivots[i] & 63)) & 1ULL) {
            for (size_t w = 0; w < word_cnt; ++w)
                tmp[w] ^= basis_vecs[i].v[w];
        }
    }
    return vec_is_zero(tmp);
}

/* ----- sequence function ------------------------------------------ */
seq_t a(idx_t n)
{
    if (!spf) init_sieve();                    /* one‑time init */

    /* grow cache if needed */
    if (n > cache_sz) {
        size_t new_sz = (n > cache_sz * 2) ? n : cache_sz * 2;
        if (new_sz == 0) new_sz = 16;
        seq_t *newc = (seq_t*)realloc(cache, new_sz * sizeof(seq_t));
        if (!newc) return 0;                 /* allocation failure */
        cache = newc;
        cache_sz = new_sz;
    }

    /* compute values up to n that are not yet cached */
    for (size_t cur = cache_len + 1; cur <= n; ++cur) {
        /* new basis for interval [cur, k] */
        basis_len = 0;

        seq_t ans = (seq_t)cur;      /* fallback if no subset found */

        for (idx_t k = cur; k <= MAX_LIMIT; ++k) {
            uint64_t v[MAX_WORDS];
            parity_vector(k, v);
            basis_insert(v);
            if (target_in_span()) {
                ans = (seq_t)k;
                break;
            }
        }
        cache[cur - 1] = ans;
    }
    cache_len = n;
    return cache[n - 1];
}
