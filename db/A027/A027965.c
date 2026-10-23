/*
 * A027965  T(n, 2*n-3), T given by A027960.
 * https://oeis.org/A027965
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (18 - 10*n + 3*n^2 + n^3)/6
 * Range:     u8 n <= 5, u16 n <= 39, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n < 2)
        return 0;  /* out of offset range, value not defined */
    seq_t nn = (seq_t)n;
    seq_t n2 = nn * nn;
    seq_t n3 = n2 * nn;
    seq_t num = n3 + 3 * n2 - (seq_t)10 * nn + (seq_t)18;
    return num / 6;
}
