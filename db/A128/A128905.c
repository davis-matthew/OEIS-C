/*
 * A128905  Numbers k such that the k-th triangular number has exactly four
 *          distinct prime factors.
 * https://oeis.org/A128905
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 38, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A128905 – indices k for which the k‑th triangular number has
   exactly four distinct prime factors and is square‑free.
   The function returns the n‑th such k (offset 1).  */

/* The implementation searches k values incrementally, keeps a
   cache of those that meet the criteria, and returns the
   requested element.  It uses a basic prime sieve up to a
   fixed limit, and checks each k by factoring the two coprime
   parts of k*(k+1)/2.  Both parts are required to be
   square‑free with a total of four distinct primes.  */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* ----------  prime sieve  ---------- */
static uint32_t *prime_tbl = NULL;
static size_t prime_cnt = 0;

#define PRIME_LIMIT 2000000   /* enough for k needed for n ≤ 10000 */

static void build_primes(void)
{
    if (prime_tbl != NULL)
        return;               /* already built */

    bool *is_comp = (bool *)calloc(PRIME_LIMIT + 1, sizeof(bool));
    if (!is_comp)
        abort();

    for (uint32_t i = 2; (uint64_t)i * i <= PRIME_LIMIT; ++i) {
        if (!is_comp[i]) {
            for (uint64_t j = (uint64_t)i * i; j <= PRIME_LIMIT; j += i)
                is_comp[(size_t)j] = true;
        }
    }

    size_t cnt = 0;
    for (uint32_t i = 2; i <= PRIME_LIMIT; ++i)
        if (!is_comp[i]) ++cnt;

    prime_tbl = (uint32_t *)malloc(cnt * sizeof(uint32_t));
    if (!prime_tbl)
        abort();

    size_t idx = 0;
    for (uint32_t i = 2; i <= PRIME_LIMIT; ++i)
        if (!is_comp[i]) prime_tbl[idx++] = i;

    prime_cnt = cnt;
    free(is_comp);
}

/* ----------  factor counting and square‑free test  ---------- */
static int dpf_and_squarefree(uint64_t n, bool *sq)
{
    int cnt = 0;
    *sq = true;
    for (size_t i = 0; i < prime_cnt; ++i) {
        uint32_t p = prime_tbl[i];
        if ((uint64_t)p * p > n)
            break;
        if (n % p == 0) {
            ++cnt;
            int e = 1;
            n /= p;
            while (n % p == 0) {
                n /= p;
                ++e;
            }
            if (e > 1)
                *sq = false;
        }
    }
    if (n > 1) {            /* remaining prime factor > sqrt(original) */
        ++cnt;
    }
    return cnt;
}

/* ----------  caching of found terms  ---------- */
static seq_t *cache = NULL;
static size_t cache_sz = 0;
static size_t cache_cap = 0;

static void ensure_cache_cap(size_t need)
{
    if (cache_cap >= need)
        return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 128;
    if (new_cap < need)
        new_cap = need;
    seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!new_cache)
        abort();
    cache = new_cache;
    cache_cap = new_cap;
}

static idx_t next_candidate = 1;   /* next k to test */

/* ----------  main function  ---------- */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;            /* undefined for offset 0 */

    build_primes();                  /* ensure sieve ready */

    while (cache_sz < (size_t)n) {
        uint64_t k = (uint64_t)next_candidate;
        bool sq1, sq2;
        int dpf1, dpf2;

        if (k & 1ULL) {              /* k odd: k * (k+1)/2 */
            dpf1 = dpf_and_squarefree(k, &sq1);
            dpf2 = dpf_and_squarefree((k + 1) / 2, &sq2);
        } else {                     /* k even */
            dpf1 = dpf_and_squarefree(k / 2, &sq1);
            dpf2 = dpf_and_squarefree(k + 1, &sq2);
        }

        if (sq1 && sq2 && (dpf1 + dpf2 == 4)) {
            ensure_cache_cap(cache_sz + 1);
            cache[cache_sz++] = (seq_t)k;
        }
        ++next_candidate;
    }
    return cache[(size_t)n - 1];
}
