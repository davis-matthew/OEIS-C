/*
 * A003622  The Wythoff compound sequence AA: a(n) = floor(n*phi^2) - 1, where
 *          phi = (1+sqrt(5))/2.
 * https://oeis.org/A003622
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = floor(n*phi) + n - 1, where phi = (1+sqrt(5))/2
 * Range:     u8 n <= 98, u16 n <= 9648, u32 n <= 9648, u64 n <= 9648,
 *            u128 n <= 9648, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t n_val = (seq_t)n;
    seq_t n_phi = n_val * 6765 / 4181;
    return n_phi + n_val - 1;
}
