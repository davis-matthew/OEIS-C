/*
 * A181192  Number of n X 5 matrices containing a permutation of 1..n*5 in
 *          increasing order rowwise, columnwise, diagonally and (downwards)
 *          antidiagonally.
 * https://oeis.org/A181192
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 24*a(n-1) - 40*a(n-2) - 8*a(n-3); Initial values: a(1)=1,
 *            a(2)=14, a(3)=290
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 15, u128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined for n<1, not used */
    if (n == 1) return 1;
    if (n == 2) return 14;
    if (n == 3) return 290;

    seq_t a1 = 1;   /* a(n-3) */
    seq_t a2 = 14;  /* a(n-2) */
    seq_t a3 = 290; /* a(n-1) */
    seq_t an = 0;

    for (idx_t i = 4; i <= n; ++i) {
        an = 24 * a3 - 40 * a2 - 8 * a1;
        a1 = a2;
        a2 = a3;
        a3 = an;
    }
    return a3;
}
