/*
 * A079272  a(n) = ((2n+1)*3^n - 1)/2.
 * https://oeis.org/A079272
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ((2n+1)*3^n - 1)/2
 * Range:     u8 n <= 3, u16 n <= 8, u32 n <= 17, u64 n <= 37, u128 n <= 76,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>  /* for idx_t if it is defined here */

seq_t a(idx_t n)
{
    if (n == 0) return 0;         /* a(0) is undefined – return 0 to avoid UB */

    /* Compute 3^n */
    seq_t pow3 = 1;
    for (idx_t i = 0; i < n; ++i) {
        pow3 *= 3;                /* multiplication by the small constant 3 */
    }

    /* a(n) = n*3^n + (3^n - 1)/2  (avoids overflow of (2n+1)*3^n) */
    seq_t term1 = pow3 * (seq_t)n;      /* n*3^n */
    seq_t term2 = pow3 - 1;             /* 3^n - 1 (even) */
    term2 /= 2;                         /* divide by 2 */

    return term1 + term2;               /* sum gives a(n) */
}
