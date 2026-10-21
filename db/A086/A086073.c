/*
 * A086073  Number of 2's in decimal expansion of triangular number n(n+1)/2.
 * https://oeis.org/A086073
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = the number of 2’s in the decimal expansion of n*(n+1)/2
 * Range:     u8 n <= 24, u16 n <= 104, u32 n <= 104, u64 n <= 104,
 *            u128 n <= 104, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    /* compute the triangular number t = n*(n+1)/2 in seq_t arithmetic */
    seq_t tri = (seq_t)n * ((seq_t)n + 1) / 2;

    /* count the digit 2 in the decimal representation of tri */
    seq_t cnt = 0;
    while (tri != 0) {
        if ((tri % 10) == 2) {
            cnt += 1;
        }
        tri /= 10;
    }
    return cnt;
}
