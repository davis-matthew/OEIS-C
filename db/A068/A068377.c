/*
 * A068377  Engel expansion of sinh(1).
 * https://oeis.org/A068377
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (2*n-2)*(2*n-1), with a(1) = 1 (offset 1)
 * Range:     u8 n <= 8, u16 n <= 128, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n <= 1) {
        return 1;  /* a(1) = 1 */
    }
    seq_t sn = (seq_t)n;          /* cast index to seq_t */
    seq_t two_n_minus2 = sn * 2 - 2;
    seq_t two_n_minus1 = sn * 2 - 1;
    return two_n_minus2 * two_n_minus1;
}
