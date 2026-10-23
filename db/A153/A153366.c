/*
 * A153366  Number of zig-zag paths from top to bottom of a rectangle of width
 *          9 with 2*n-1 rows whose color is that of the top right corner.
 * https://oeis.org/A153366
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 5*a(n‑1) - 5*a(n‑2) for n ≥ 4, with initial values
 *            a(1)=5, a(2)=16, a(3)=56
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 17, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n < 1) return (seq_t)0;
    if (n == 1) return (seq_t)5;
    if (n == 2) return (seq_t)16;
    if (n == 3) return (seq_t)56;

    seq_t prev2 = (seq_t)16; /* a(2) */
    seq_t prev1 = (seq_t)56; /* a(3) */
    seq_t cur   = 0;

    for (idx_t i = 4; i <= n; ++i) {
        cur   = 5 * prev1 - 5 * prev2;
        prev2 = prev1;
        prev1 = cur;
    }
    return cur;
}
