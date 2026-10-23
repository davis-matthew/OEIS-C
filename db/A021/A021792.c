/*
 * A021792  Decimal expansion of 1/788.
 * https://oeis.org/A021792
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons
 * Formula:   a(n) = the (n+1)-th decimal digit of 1/788 (0-indexed)
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* On first call we create an array big enough to store digits
       up to n. For subsequent calls we extend the array if needed
       and compute only the missing digits.  */
    static seq_t *digits = NULL;      /* array of digits 0..9 */
    static size_t cap = 0;            /* current capacity */
    static idx_t  max_cached = 0;     /* number of digits stored so far */
    static uint64_t rem = 1;          /* remainder after last computed digit */

    /* Extend capacity if required */
    if (cap <= (size_t)n) {
        size_t newcap = cap ? cap * 2 : 1;
        while (newcap <= (size_t)n) newcap <<= 1;
        digits = (seq_t *)realloc(digits, newcap * sizeof(seq_t));
        cap = newcap;
    }

    /* Compute missing digits up to index n (inclusive) */
    for (idx_t i = max_cached; i <= n; ++i) {
        uint64_t t = rem * 10ULL;
        seq_t d = (seq_t)(t / 788ULL);
        rem   = t % 788ULL;
        digits[i] = d;
    }

    max_cached = n + 1;   /* now digits 0..n are stored */
    return digits[n];
}
