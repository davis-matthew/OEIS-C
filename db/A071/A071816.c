/*
 * A071816  Number of ordered solutions to x+y+z = u+v+w, 0 <= x, y, z, u, v,
 *          w < n.
 * https://oeis.org/A071816
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = n*(11*n^4 + 5*n^2 + 4)/20 (valid for n >= 1)
 * Range:     u8 n <= 3, u16 n <= 10, u32 n <= 95, u64 n <= 8037,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0) return 0;          /* safety for out-of-range calls */
    unsigned __int128 N = (unsigned __int128)n;
    unsigned __int128 N2 = N * N;          /* n^2   */
    unsigned __int128 N4 = N2 * N2;        /* n^4   */
    unsigned __int128 temp = 11 * N4 + 5 * N2 + 4;
    unsigned __int128 RES = N * temp / 20; /* division is exact */
    return (seq_t)RES;
}
