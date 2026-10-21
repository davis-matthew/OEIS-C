/*
 * A203152  (n-1)-st elementary symmetric function of {1, 2, 2, 3, 3, 4, 4, 5,
 *          5, ..., floor(1+n/2)}.
 * https://oeis.org/A203152
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n-1)-st elementary symmetric function of {1, 2, 2, 3, 3,
 *            4, 4, 5, 5, …, floor(1+n/2)}. The function returns the nth term
 *            (with a(1)=1)
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 14, u64 n <= 24, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* safety for out-of-range */
    if (n == 1) return (seq_t)1;   /* a(1) = 1 */

    /* Build the list of the first n elements of the multiset */
    idx_t *vals = (idx_t *)malloc((size_t)n * sizeof(idx_t));
    if (!vals) return (seq_t)0;     /* allocation failed – unlikely */

    vals[0] = (idx_t)1;
    idx_t next = 2;                /* start with value 2 */
    idx_t pos = 1;                 /* next free position in vals */

    while (pos < n) {
        vals[pos++] = next;
        if (pos >= n) break;
        vals[pos++] = next;
        if (pos >= n) break;
        ++next;
    }

    /* Compute the product P of all elements */
    seq_t P = (seq_t)1;
    for (idx_t i = 0; i < n; ++i) {
        P *= (seq_t)vals[i];
    }

    /* Compute the sum of products of all (n-1)-subsets via
       successive adjustment of the term excluding the current element. */
    seq_t term = P / (seq_t)vals[0];  /* product of all except first */
    seq_t sum  = term;
    for (idx_t i = 0; i + 1 < n; ++i) {
        term = term * (seq_t)vals[i] / (seq_t)vals[i + 1];
        sum += term;
    }

    free(vals);
    return sum;
}
