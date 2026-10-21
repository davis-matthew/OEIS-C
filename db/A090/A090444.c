/*
 * A090444  Fifth column (m=4) of triangle A090441.
 * https://oeis.org/A090444
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n!)*(n+1)!*(n+2)!*(n+3)! / 12
 * Range:     u8 n <= 0, u16 n <= 2, u32 n <= 3, u64 n <= 6, u128 n <= 10,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

/* Compute a(n).  The calculation uses only integer arithmetic
   and works for any seq_t that supports + - * / and conversion
   from idx_t to seq_t. */
seq_t a(idx_t n)
{
    /* Factorials: f0 = n!, f1 = (n+1)!, f2 = (n+2)!, f3 = (n+3)! */
    seq_t f0 = 1;
    for (idx_t i = 1; i <= n; ++i)
        f0 *= (seq_t)i;                       /* f0 = n! */

    seq_t f1 = f0 * (seq_t)(n + 1);            /* f1 = (n+1)! */
    seq_t f2 = f1 * (seq_t)(n + 2);            /* f2 = (n+2)! */
    seq_t f3 = f2 * (seq_t)(n + 3);            /* f3 = (n+3)! */

    /* Multiply all four factorials then divide by 12. */
    seq_t prod = f0;
    prod *= f1;
    prod *= f2;
    prod *= f3;
    prod /= (seq_t)12;                          /* exact division */

    return prod;
}
