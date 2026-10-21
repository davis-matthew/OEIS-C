/*
 * A227380  Doubling the first two of every four nonnegative numbers.
 * https://oeis.org/A227380
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(4k) = 8k * a(4k+1) = 8k+2 * a(4k+2) = 4k+2 * a(4k+3) = 4k+3 *
 *            (where k = ⌊n/4⌋)
 * Range:     u8 n <= 57, u16 n <= 57, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    idx_t k = n / 4;
    idx_t r = n & 3;  /* r = n % 4 */
    seq_t res;
    if (r == 0) {
        res = (seq_t)8 * (seq_t)k;
    } else if (r == 1) {
        res = (seq_t)8 * (seq_t)k + (seq_t)2;
    } else if (r == 2) {
        res = (seq_t)4 * (seq_t)k + (seq_t)2;
    } else { /* r == 3 */
        res = (seq_t)4 * (seq_t)k + (seq_t)3;
    }
    return res;
}
