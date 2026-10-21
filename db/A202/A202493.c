/*
 * A202493  Column k = 4 of triangular array in A165241.
 * https://oeis.org/A202493
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2^n * (n+4) * (27*n^3 + 126*n^2 + 169*n + 62) / 256 (n>0)
 * Range:     u8 n <= 0, u16 n <= 2, u32 n <= 17, u64 n <= 45, u128 n <= 104,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(0) = 1 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;

    /* compute the polynomial part: 27*n^3 + 126*n^2 + 169*n + 62 */
    seq_t nn  = (seq_t)n;
    seq_t nn2 = nn * nn;      /* n^2   */
    seq_t nn3 = nn2 * nn;     /* n^3   */

    seq_t poly = (seq_t)27 * nn3
               + (seq_t)126 * nn2
               + (seq_t)169 * nn
               + (seq_t)62;

    /* (n+4) factor */
    seq_t part = (seq_t)(n + 4);

    /* product (n+4)*poly is always < 2^64 for n where the final result
       itself fits into seq_t, so the multiplication cannot overflow a
       64‑bit intermediate under those conditions. */
    seq_t product = part * poly;

    if (n < 8) {
        /* use the original formula: 2^n * product / 256 */
        seq_t power = pow2(n);                /* 2^n */
        seq_t tmp   = product * power;        /* 2^n * (n+4)*poly */
        return tmp / (seq_t)256;              /* exact integer division */
    } else {
        /* cancel the 256 with 2^n:  2^n / 256 = 2^(n-8) */
        seq_t power = pow2(n - 8);            /* 2^(n-8) */
        return product * power;               /* exact value */
    }
}
