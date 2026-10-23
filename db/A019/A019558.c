/*
 * A019558  Coordination sequence for F_4 lattice.
 * https://oeis.org/A019558
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 16*n*(2 - 3*n + 4*n^2) for n ≥ 1, a(0) = 1
 * Range:     u8 n <= 1, u16 n <= 10, u32 n <= 406, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) {
        return 1;
    }
    seq_t nn = (seq_t)n;
    /* Compute the inner expression: 4*n^2 - 3*n + 2 */
    seq_t inner = (4 * nn * nn) - (3 * nn) + 2;
    /* Final result: 16 * n * inner */
    return 16 * nn * inner;
}
