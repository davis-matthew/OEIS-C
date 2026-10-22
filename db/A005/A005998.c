/*
 * A005998  Number of paraffins.
 * https://oeis.org/A005998
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(n^2 - n + 2)/4 for even n; a(n) = (n^3 - n^2 + 3*n +
 *            1)/4 for odd n
 * Range:     u8 n <= 6, u16 n <= 40, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t nn = (seq_t)n;
    
    if (n % 2 == 0) {
        // even: a(n) = (n^3 - n^2 + 2*n) / 4
        seq_t nsq = nn * nn;
        seq_t ncube = nsq * nn;
        return (ncube - nsq + 2 * nn) / 4;
    } else {
        // odd: a(n) = (n^3 - n^2 + 3*n + 1) / 4
        seq_t nsq = nn * nn;
        seq_t ncube = nsq * nn;
        return (ncube - nsq + 3 * nn + 1) / 4;
    }
}
