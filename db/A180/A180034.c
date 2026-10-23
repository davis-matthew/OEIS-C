/*
 * A180034  Eight white queens and one red queen on a 3 X 3 chessboard. G.f.:
 *          (1 - 2*x)/(1 - 6*x + 2*x^2).
 * https://oeis.org/A180034
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 6*a(n-1) - 2*a(n-2) with a(0) = 1, a(1) = 4
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 13, u64 n <= 25, u128 n <= 51,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* External linkage as required */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)4;

    seq_t prev2 = (seq_t)1;     /* a(0) */
    seq_t prev1 = (seq_t)4;     /* a(1) */
    seq_t current = 0;

    for (idx_t i = 2; i <= n; ++i) {
        current = (seq_t)6 * prev1 - (seq_t)2 * prev2;
        prev2 = prev1;
        prev1 = current;
    }
    return current;
}
