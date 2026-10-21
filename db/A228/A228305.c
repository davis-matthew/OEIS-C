/*
 * A228305  a(1) = 3; for n >= 1, a(2*n) = 2^(n+1), a(2*n+1) = 5*2^(n-1).
 * https://oeis.org/A228305
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(1)=3; for n≥1, a(2n)=2^(n+1), a(2n+1)=5·2^(n-1)
 * Range:     u8 n <= 13, u16 n <= 29, u32 n <= 43, u64 n <= 43, u128 n <= 43,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Equivalent recurrence: a(1)=3, a(2)=4, a(3)=5, a(n)=2·a(n-2) for n>3 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* n is 1‑based */
    if (n == 1)
        return (seq_t)3;

    if ((n & 1) == 0) {              /* even n = 2*n' */
        idx_t n2  = n >> 1;           /* n' = n/2 */
        idx_t exp = n2 + 1;           /* 2^(n'+1) */
        return ((seq_t)1 << exp);     /* 1 shifted left by exp bits */
    }
    else {                            /* odd n = 2*n'+1, n'≥1 */
        idx_t n2  = (n - 1) >> 1;     /* n' = (n-1)/2 */
        idx_t exp = (n2 > 0) ? n2 - 1 : 0; /* exponent for 2^(n'-1) */
        return ((seq_t)5 << exp);     /* 5 shifted left by exp bits */
    }
}
