/*
 * A222240  In the number n, replace all (decimal) digits '4' with '5' and
 *          vice versa.
 * https://oeis.org/A222240
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = decimal digit map swapping digits 4 and 5
 * Range:     u8 n <= 245, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    idx_t x = n;
    seq_t res = 0;
    seq_t pow10 = 1;
    while (x > 0) {
        unsigned digit = (unsigned)(x % 10);
        unsigned nd = (digit == 4 ? 5 : (digit == 5 ? 4 : digit));
        res += (seq_t)nd * pow10;
        pow10 *= 10;
        x /= 10;
    }
    return res;
}
