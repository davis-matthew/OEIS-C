/*
 * A172213  Number of ways to place 4 nonattacking knights on a 4 X n board.
 * https://oeis.org/A172213
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 8*(4*n^4-36*n^3+170*n^2-450*n+537)/3 for n ≥ 6 Initial
 *            values: a(1)=1, a(2)=16, a(3)=84, a(4)=412, a(5)=1416
 * Range:     u8 n <= 3, u16 n <= 10, u32 n <= 125, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)16;
    if (n == 3) return (seq_t)84;
    if (n == 4) return (seq_t)412;
    if (n == 5) return (seq_t)1416;
    /* n ≥ 6 */
    seq_t nn = (seq_t)n;
    /* Compute polynomial 4*n^4 - 36*n^3 + 170*n^2 - 450*n + 537 */
    seq_t term1 = 4 * nn * nn * nn * nn; /* 4*n^4 */
    seq_t term2 = 36 * nn * nn * nn;     /* 36*n^3 */
    seq_t term3 = 170 * nn * nn;         /* 170*n^2 */
    seq_t term4 = 450 * nn;              /* 450*n   */
    seq_t poly = term1 - term2 + term3 - term4 + 537;
    /* Multiply by 8 and divide by 3 (division is exact) */
    return (8 * poly) / 3;
}
