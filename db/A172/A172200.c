/*
 * A172200  Number of ways to place 2 nonattacking amazons (superqueens) on an
 *          n X n board.
 * https://oeis.org/A172200
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n - 1)(n - 2)(n - 3)(3n + 8)/6
 * Range:     u8 n <= 4, u16 n <= 13, u32 n <= 219, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* For n < 4 the product contains a zero factor and the result is 0. */
    seq_t nn = (seq_t)n;
    seq_t a1 = nn - 1;
    seq_t a2 = nn - 2;
    seq_t a3 = nn - 3;
    seq_t a4 = 3 * nn + 8;

    seq_t prod = a1 * a2;
    prod = prod * a3;
    prod = prod * a4;

    return prod / 6;
}
