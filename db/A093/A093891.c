/*
 * A093891  Numbers k such that every prime up to sigma(k) is a sum of
 *          divisors of k.
 * https://oeis.org/A093891
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the n-th integer k such that every prime ≤ σ(k) is a sum
 *            of distinct divisors of k. Computed by searching k and testing
 *            the property with a bit‑set subset–sum routine
 * Range:     u8 n <= 62, u16 n <= 8337, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

/* ---------- prime sieve ------------ */
struct prime_sieve {
    size_t limit;       /* largest value sieved (inclusive) */
    bool  *is_prime;    /* boolean sieve array */
    int   *primes;      /* list of primes */
    size_t prime_cnt;   /* number of primes stored */
};

static struct prime_sieve sieve = {0, NULL, NULL, 0};

static void init_sieve(size_t lim)
{
    if (sieve.limit >= lim) return;   /* already big enough */

    if (sieve.is_prime) free(sieve.is_prime);
    if (sieve.primes)   free(sieve.primes);

    sieve.is_prime = (bool *)calloc(lim + 1, sizeof(bool));
    if (!sieve.is_prime) exit(EXIT_FAILURE);
    for (size_t i = 2; i <= lim; ++i) sieve.is_prime[i] = true;

    for (size_t i = 2; i * i <= lim; ++i) {
        if (sieve.is_prime[i]) {
            for (size_t j = i * i; j <= lim; j += i)
                sieve.is_prime[j] = false;
        }
    }

    size_t cnt = 0;
    for (size_t i = 2; i <= lim; ++i)
        if (sieve.is_prime[i]) ++cnt;

    sieve.primes = (int *)malloc(cnt * sizeof(int));
    if (!sieve.primes) exit(EXIT_FAILURE);
    size_t idx = 0;
    for (size_t i = 2; i <= lim; ++i)
        if (sieve.is_prime[i]) sieve.primes[idx++] = (int)i;

    sieve.prime_cnt = cnt;
    sieve.limit = lim;
}

/* ---------- bit‑set helper ---------- */
static uint64_t *bitset_arr = NULL;
static size_t   bitset_words = 0;

static void ensure_bitset(size_t W)
{
    if (W <= bitset_words) {
        memset(bitset_arr, 0, bitset_words * sizeof(uint64_t));
        return;
    }
    free(bitset_arr);
    bitset_arr = (uint64_t *)calloc(W, sizeof(uint64_t));
    if (!bitset_arr) exit(EXIT_FAILURE);
    bitset_words = W;
}

static void shift_or(uint64_t *dst, const uint64_t *src, size_t W, uint64_t shift)
{
    size_t word_shift = shift >> 6;       /* shift / 64 */
    uint64_t bit_shift = shift & 63;      /* shift % 64 */

    if (bit_shift == 0) {
        for (size_t i = W; i-- > word_shift;) {
            dst[i] |= src[i - word_shift];
        }
    } else {
        for (size_t i = W; i-- > word_shift;) {
            uint64_t val = src[i - word_shift] << bit_shift;
            if (i - word_shift > 0)
                val |= src[i - word_shift - 1] >> (64 - bit_shift);
            dst[i] |= val;
        }
    }
}

/* ---------- cache for the sequence values ---------- */
static seq_t  *cache_vals   = NULL;
static size_t  cache_len   = 0;
static size_t  cache_cap   = 0;
static uint64_t last_checked = 1;  /* last candidate examined */

static void cache_append(seq_t v)
{
    if (cache_len >= cache_cap) {
        size_t new_cap = (cache_cap == 0) ? 16 : cache_cap * 2;
        seq_t *new_arr = (seq_t *)realloc(cache_vals, new_cap * sizeof(seq_t));
        if (!new_arr) exit(EXIT_FAILURE);
        cache_vals = new_arr;
        cache_cap = new_cap;
    }
    cache_vals[cache_len++] = v;
}

/* ---------- test a single candidate k ---------- */
static bool candidate_ok(uint64_t k)
{
    if ((k & (k - 1)) == 0)          /* k is a power of two ⇒ trivially true */
        return true;

    /* compute sigma(k) */
    uint64_t sigma = 0;
    for (uint64_t d = 1; d * d <= k; ++d)
        if (k % d == 0) {
            sigma += d;
            uint64_t e = k / d;
            if (e != d) sigma += e;
        }

    /* ensure prime sieve covers primes up to sigma */
    if (sigma > (uint64_t)sieve.limit)
        init_sieve((size_t)sigma + 1);
    if (!sieve.is_prime)
        init_sieve(4000);   /* initial small sieve */

    /* subset‑sum DP on all divisors */
    size_t L = sigma + 1;                           /* need bits 0..sigma */
    size_t W = (L + 63) >> 6;                       /* number of 64‑bit words */
    ensure_bitset(W);
    bitset_arr[0] = 1;                              /* sum 0 reachable */

    for (uint64_t d = 1; d * d <= k; ++d)
        if (k % d == 0) {
            shift_or(bitset_arr, bitset_arr, W, d);
            uint64_t e = k / d;
            if (e != d)
                shift_or(bitset_arr, bitset_arr, W, e);
        }

    /* check all primes <= sigma are reachable sums */
    for (size_t pi = 0; pi < sieve.prime_cnt && sieve.primes[pi] <= sigma; ++pi) {
        int p = sieve.primes[pi];
        size_t w = p >> 6;
        uint64_t mask = 1ULL << (p & 63);
        if ((bitset_arr[w] & mask) == 0)
            return false;
    }
    return true;
}

/* ---------- main sequence function ---------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;        /* undefined but safe */

    /* initialize sieve once */
    if (!sieve.is_prime) init_sieve(4000);

    /* extend cache until we have n terms */
    while (cache_len < n) {
        while (!candidate_ok(last_checked))
            ++last_checked;
        cache_append((seq_t)last_checked);
        ++last_checked;
    }

    return cache_vals[n - 1];
}
