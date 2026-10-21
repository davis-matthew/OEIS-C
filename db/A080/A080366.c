/*
 * A080366  Numbers k whose least and greatest prime divisors are non-unitary.
 * https://oeis.org/A080366
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = nth integer k such that the exponent of the smallest
 *            prime factor of k and the exponent of the largest prime factor
 *            of k are both ≥ 2. (Computed by incremental search.)
 * Range:     u8 n <= 24, u16 n <= 800, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>

/*---  Cache for computed terms  -----------------------------------------*/
static seq_t *cache      = NULL;  /* array of cached values          */
static size_t cache_len  = 0;      /* number of cached terms          */
static size_t cache_cap  = 0;      /* allocated capacity of cache     */
static uint64_t tested   = 1;      /* last integer examined          */

/* Ensure that the cache can hold at least `need` elements. */
static void ensure_cache_capacity(size_t need)
{
    if (cache_cap >= need) return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 8;
    while (new_cap < need) new_cap *= 2;
    seq_t *newptr = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    cache = newptr;
    cache_cap = new_cap;
}

/* Determine whether a positive integer v qualifies:
   both the smallest and the largest prime divisors occur with exponent ≥ 2. */
static bool qualifies(uint64_t v)
{
    if (v < 4) return false;               /* smallest qualifying number is 4 */
    uint64_t n = v;
    unsigned minExp = 0;   /* exponent of smallest prime factor   */
    unsigned maxExp = 0;   /* exponent of largest  prime factor   */

    /* Handle factor 2 separately */
    unsigned exp = 0;
    while ((n & 1ULL) == 0) {
        n >>= 1;
        ++exp;
    }
    if (exp > 0) {
        minExp = exp;
        maxExp = exp;
    }

    /* Trial division by odd integers */
    for (uint64_t p = 3; p * p <= n; p += 2) {
        if (n % p == 0) {
            exp = 0;
            while (n % p == 0) {
                n /= p;
                ++exp;
            }
            if (minExp == 0) {   /* first prime factor found */
                minExp = exp;
            }
            maxExp = exp;           /* last prime factor seen so far */
        }
    }

    /* Remaining prime factor > sqrt(original n) */
    if (n > 1) {
        exp = 1;
        if (minExp == 0) {     /* the number itself is prime */
            minExp = exp;
        }
        maxExp = exp;
    }

    return (minExp >= 2) && (maxExp >= 2);
}

/*---  Sequence a(n)  -----------------------------------------------------*/
seq_t a(idx_t n)
{
    if (n == 0) return 0;        /* offset is 1; unused */

    /* Grow the cache until it holds at least the n‑th term. */
    if (cache_len < (size_t)n) {
        ensure_cache_capacity((size_t)n);
        while (cache_len < (size_t)n) {
            ++tested;                       /* move to next integer   */
            if (qualifies(tested)) {
                cache[cache_len++] = (seq_t)tested;
            }
        }
    }
    return cache[(size_t)n - 1];
}
