/*
 * A145801  Number of islands of ones fitting in an n X n array with all ones
 *          connected only either two adjacent vertically or two adjacent
 *          horizontally.
 * https://oeis.org/A145801
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor(n^2/4) if n is even * a(n) = floor(n^2/4) -
 *            floor(n/8) if n is odd
 * Range:     u8 n <= 12, u16 n <= 12, u32 n <= 12, u64 n <= 12, u128 n <= 12,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t nsq = (seq_t)n * (seq_t)n;
    seq_t res = nsq / 4;               /* floor(n^2/4) */
    if (n & 1) {
        res -= (seq_t)(n / 8);          /* subtract floor(n/8) for odd n */
    }
    return res;
}
