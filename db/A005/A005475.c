/*
 * A005475  a(n) = n*(5*n+1)/2.
 * https://oeis.org/A005475
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(5*n+1)/2
 * Range:     u8 n <= 10, u16 n <= 161, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t nn = (seq_t)n;
    seq_t five_nn_plus_one = 5 * nn + 1;
    if (nn % 2 == 0) {
        return (nn / 2) * five_nn_plus_one;
    } else {
        return nn * (five_nn_plus_one / 2);
    }
}
