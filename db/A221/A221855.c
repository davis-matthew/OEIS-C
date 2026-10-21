/*
 * A221855  Number of cyclotomic cosets of 13 mod 10^n.
 * https://oeis.org/A221855
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(1)=4, a(2)=12, a(n)=2*n*(4*n-7)=8*n^2-14*n for n≥3
 * Range:     u8 n <= 6, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* below offset, behavior irrelevant */
    if (n == 1) return 4;
    if (n == 2) return 12;

    seq_t nn = (seq_t)n;
    seq_t res = 8 * nn * nn;   /* 8 * n^2 */
    res -= 14 * nn;            /* subtract 14 * n */
    return res;
}
