/*
 * A126388  Denominators in a series for the "alternating Euler constant"
 *          log(4/Pi).
 * https://oeis.org/A126388
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 72, u16 n <= 72, u32 n <= 72, u64 n <= 72, u128 n <= 72,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A126388 – Denominators in a series for the "alternating Euler constant" log(4/Pi)
 * Definition: a(n) = the n‑th number m>1 such that the binary expansion of floor(m/2)
 * has a different number of 1's and 0's.
 * Offset: 2  (i.e. a(2) is the first term listed in OEIS)
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

static inline bool qualifies(idx_t m) {
    /* m > 1 */
    idx_t x = m / 2;                 /* floor(m/2) */
    if (x == 0) return false;        /* defensive, should not happen for m>1 */
    unsigned long long ux = (unsigned long long)x;
    unsigned pc = __builtin_popcountll(ux);          /* number of 1 bits */
    int bitlen = 64 - __builtin_clzll(ux);           /* number of bits in binary rep */
    int zeros = bitlen - pc;
    return pc != zeros;
}

seq_t a(idx_t n) {
    if (n < 2) return 0;             /* undefined for n < 2, return 0 as guard */

    /* 0‑based index of the term in the cache: a(2) -> index 0 */
    idx_t target = n - 2;

    /* Static cache of already computed terms */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;
    static idx_t candidate = 2;      /* next integer to test */

    /* Ensure the cache is large enough for the requested term */
    while ((size_t)cache_len <= (size_t)target) {
        /* Find the next qualifying integer */
        while (!qualifies(candidate))
            ++candidate;

        /* Grow cache if necessary */
        if (cache_len == cache_cap) {
            size_t new_cap = cache_cap == 0 ? 8 : cache_cap * 2;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) abort();          /* memory allocation failed */
            cache = tmp;
            cache_cap = new_cap;
        }

        /* Store the qualifying integer */
        cache[cache_len++] = (seq_t)candidate;
        ++candidate;                   /* advance to the next number for future calls */
    }

    return cache[(size_t)target];
}
