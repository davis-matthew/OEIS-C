/*
 * A220020  Number of cyclotomic cosets of 9 mod 10^n.
 * https://oeis.org/A220020
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = 4*(4*n^2-7*n-1) for n>2; a(1)=6, a(2)=20
 * Range:     u8 n <= 4, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implements OEIS sequence A220020: Number of cyclotomic cosets of 9 mod 10^n. */
#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 1) return 6;
    if (n == 2) return 20;

    seq_t nn = (seq_t)n;          /* cast to seq_t as required */
    seq_t tmp = 4 * nn * nn;      /* 4*n^2 */
    tmp -= 7 * nn;                /* 4*n^2 - 7*n */
    tmp -= 1;                     /* 4*n^2 - 7*n - 1 */
    return 4 * tmp;               /* multiply by 4 */
}
