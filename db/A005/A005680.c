/*
 * A005680  A squarefree ternary sequence.
 * https://oeis.org/A005680
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = s(n-1) with s the fixed point of the 4-uniform morphism
 *            1->12x1, 2->21y2, x->x1y2, y->y2x1, starting from 1; x and y are
 *            both written as 3 (x is a 3 followed by 1, y is a 3 followed by
 *            2)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static const int A005680_h[5][4] = {
    {0, 0, 0, 0},
    {1, 2, 3, 1},   /* 1 -> 1 2 x 1 */
    {2, 1, 4, 2},   /* 2 -> 2 1 y 2 */
    {3, 1, 4, 2},   /* x -> x 1 y 2 */
    {4, 2, 3, 1}    /* y -> y 2 x 1 */
};

static int A005680_sym(uint64_t m) {
    if (m == 0) return 1;
    return A005680_h[A005680_sym(m / 4)][m % 4];
}

seq_t a(idx_t n) {
    int s;
    if (n < 1) return 0;
    s = A005680_sym((uint64_t)(n - 1));
    return (seq_t)(s >= 3 ? 3 : s);
}
