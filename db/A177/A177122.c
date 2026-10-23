/*
 * A177122  Sequence defined by the recurrence formula
 *          a(n+1)=sum(a(p)*a(n-p)+k,p=0..n)+l for n>=1, with here a(0)=1,
 *          a(1)=6, k=1 and l=1.
 * https://oeis.org/A177122
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = sum_{p=0}^{n-1} a(p)*a(n-1-p) + n + 1, with a(0) = 1,
 *            a(1) = 6. This implements the recurrence defined in OEIS A177122
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 13, u64 n <= 24, u128 n <= 48,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *seq = NULL;   /* cached values */
    static idx_t  max  = 0;     /* highest index currently computed */
    static size_t cap  = 0;     /* current capacity of the array */

    /* initialise for the two base values */
    if (cap == 0) {
        cap = 2;
        seq = (seq_t *)calloc(cap, sizeof(seq_t));
        if (!seq) return (seq_t)0;    /* allocation failed */
        seq[0] = (seq_t)1;
        seq[1] = (seq_t)6;
        max   = 1;
    }

    /* if the requested value is already known, return it */
    if (n <= max) return seq[n];

    /* grow the array if necessary */
    if ((size_t)(n + 1) > cap) {
        size_t new_cap = cap ? cap : 2;
        while ((size_t)(n + 1) > new_cap)
            new_cap <<= 1;              /* double until large enough */

        seq_t *tmp = (seq_t *)realloc(seq, new_cap * sizeof(seq_t));
        if (!tmp) return (seq_t)0;       /* allocation failed */
        /* zero the new part for safety (optional for big ints) */
        for (size_t i = cap; i < new_cap; ++i) tmp[i] = (seq_t)0;
        seq = tmp;
        cap = new_cap;
    }

    /* compute all values up to n */
    for (idx_t i = max + 1; i <= n; ++i) {
        seq_t sum = (seq_t)0;
        for (idx_t p = 0; p < i; ++p) {
            sum += seq[p] * seq[i - 1 - p];
        }
        seq[i] = sum + (seq_t)(i + 1);
    }

    max = n;
    return seq[n];
}
