/*
 * A239031  Number of 4 X n 0..2 arrays with no element equal to the sum of
 *          elements to its left or one plus the sum of the elements above it,
 *          modulo 3.
 * https://oeis.org/A239031
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n^4 - 2 n^3 + 47 n^2 - 58 n + 60)/12
 * Range:     u8 n <= 6, u16 n <= 29, u32 n <= 210, u64 n <= 210,
 *            u128 n <= 210, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    /* Use signed 128‑bit integer for safe intermediate arithmetic */
    __int128 N  = (__int128)n;
    __int128 N2 = N * N;        /* n²   */
    __int128 N3 = N2 * N;       /* n³   */
    __int128 N4 = N3 * N;       /* n⁴   */
    __int128 num = N4 - (__int128)2 * N3 + (__int128)47 * N2 - (__int128)58 * N + 60;
    return (seq_t)(num / 12);
}
