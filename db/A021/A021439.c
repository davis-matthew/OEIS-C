/*
 * A021439  Decimal expansion of 1/435.
 * https://oeis.org/A021439
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons
 * Formula:   not stated
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Computation: a(n) = floor(10^n / 435) mod 10
   (digit n of the decimal expansion of 1/435) */

seq_t a(idx_t n)
{
    /* static cache of computed digits */
    static seq_t *digits = NULL;
    static size_t  cap      = 0;
    static idx_t   max_idx  = 0;
    static uint64_t rem     = 1;   /* remainder before computing digit */

    /* initialise the cache on first call */
    if (cap == 0) {
        cap = 64;
        digits = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!digits) return (seq_t)0;
        seq_t dig = (seq_t)((rem * 10ULL) / 435ULL);
        digits[0] = dig;
        rem       = (rem * 10ULL) % 435ULL;
        max_idx   = 0;
    }

    /* return cached value if already computed */
    if (n <= max_idx) return digits[n];

    /* grow cache if necessary */
    if ((size_t)n >= cap) {
        size_t new_cap = cap;
        while ((size_t)n >= new_cap) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(digits, new_cap * sizeof(seq_t));
        if (!tmp) return (seq_t)0;
        digits = tmp;
        cap    = new_cap;
    }

    /* compute new digits up to n */
    for (idx_t i = max_idx + 1; i <= n; ++i) {
        seq_t dig = (seq_t)((rem * 10ULL) / 435ULL);
        digits[i] = dig;
        rem       = (rem * 10ULL) % 435ULL;
    }
    max_idx = n;
    return digits[n];
}
