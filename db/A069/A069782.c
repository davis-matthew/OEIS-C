/*
 * A069782  Numbers k such that gcd(d(k^3), d(k)) = 2^w for some w.
 * https://oeis.org/A069782
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 255, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the n‑th term of OEIS A069782:
 *   Numbers k such that gcd(d(k^3), d(k)) is a power of two.
 * The function is implemented by sequentially testing integers,
 * caching the good ones, and returning the requested term.
 * It lazily builds the sequence up to the requested n whenever
 * a larger n is queried.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* ------- helper: gcd for seq_t -------
 */
static inline seq_t gcd_seq_t(seq_t a, seq_t b)
{
    while (b) {
        seq_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

/* ------- check whether k satisfies the property -------
 */
static bool good_k(uint64_t k)
{
    uint64_t n = k;
    uint64_t d = 1;      /* divisor count of k       */
    uint64_t d3 = 1;     /* divisor count of k^3     */

    /* factor k and accumulate divisor counts */
    for (uint64_t p = 2; p * p <= n; ++p) {
        if (n % p == 0) {
            int exp = 0;
            while (n % p == 0) {
                n /= p;
                ++exp;
            }
            d  *= (uint64_t)(exp + 1);
            d3 *= (uint64_t)(3 * exp + 1);
        }
    }
    if (n > 1) {    /* remaining prime factor with exponent 1 */
        d  *= 2;
        d3 *= 4;
    }

    seq_t g = gcd_seq_t((seq_t)d, (seq_t)d3);
    return (g & (g - 1)) == 0;   /* true iff g is a power of two */
}

/* ------- lazy cache of sequence --------------------- */
static seq_t *cache_terms      = NULL;   /* array of terms */
static size_t  cache_len       = 0;      /* number of stored terms */
static size_t  cache_alloc     = 0;      /* allocated capacity */
static uint64_t next_k_to_test = 1;      /* next integer to examine */

/* enlarge array if necessary */
static void ensure_cache_capacity(size_t needed)
{
    if (needed <= cache_alloc) return;
    size_t new_alloc = cache_alloc ? cache_alloc * 2 : 16;
    if (new_alloc < needed) new_alloc = needed;
    seq_t *tmp = (seq_t *)realloc(cache_terms, new_alloc * sizeof(seq_t));
    if (!tmp) abort();
    cache_terms = tmp;
    cache_alloc = new_alloc;
}

/* public function required by the test harness */
seq_t a(idx_t n)
{
    /* Validate input */
    if (n == 0) return 0;   /* undefined, return 0 for safety */

    /* Extend cache until it contains at least n terms */
    while (cache_len < n) {
        if (good_k(next_k_to_test)) {
            ensure_cache_capacity(cache_len + 1);
            cache_terms[cache_len++] = (seq_t)next_k_to_test;
        }
        ++next_k_to_test;
    }

    return cache_terms[n - 1];
}
