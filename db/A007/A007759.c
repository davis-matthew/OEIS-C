/*
 * A007759  Knopfmacher expansion of sqrt(2): a(2n) = 2*(a(2n-1) + 1)^2 - 1,
 *          a(2n+1) = 2*(a(2n)^2 - 1).
 * https://oeis.org/A007759
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 2; a(2n) = 2*(a(2n-1) + 1)^2 - 1, a(2n+1) = 2*(a(2n)^2 -
 *            1)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 4, u64 n <= 5, u128 n <= 6,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t x = 2;
    idx_t i;
    for (i = 2; i <= n; i++) {
        if (i % 2 == 0) {
            seq_t t = x + 1;
            x = 2 * t * t - 1;
        } else {
            x = 2 * (x * x - 1);
        }
    }
    return x;
}
