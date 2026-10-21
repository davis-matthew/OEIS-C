/*
 * A053344  Minimal number of coins needed to pay n cents using coins of
 *          denominations 1, 5, 10, 25 cents.
 * https://oeis.org/A053344
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = floor(n/25) + floor((n%25)/10) + floor(((n%25)%10)/5) +
 *            ((n%25)%10)%5
 * Range:     u8 n <= 290, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t nn = (seq_t)n;
    seq_t q25 = nn / 25;
    seq_t r25 = nn % 25;
    seq_t q10 = r25 / 10;
    seq_t r10 = r25 % 10;
    seq_t q5  = r10 / 5;
    seq_t r5  = r10 % 5;
    return q25 + q10 + q5 + r5;
}
