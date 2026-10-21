/*
 * A071241  Arithmetic mean of k and R(k) where k is the n-th nonnegative
 *          number using only even digits and R(k) is its digit reversal
 *          (A004086).
 * https://oeis.org/A071241
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = (k_n + R(k_n))/2, where k_n is the n‑th nonnegative
 *            integer whose decimal digits are only 0,2,4,6,8 and R(k) is the
 *            decimal digit reversal of k. k_n can be obtained by writing n in
 *            base 5 and interpreting each base‑5 digit d as 2·d in decimal
 * Range:     u8 n <= 26, u16 n <= 1472, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Build k_n from the base-5 digits of n */
    seq_t k = 0;          /* the desired number with only 0,2,4,6,8 digits   */
    seq_t pow10 = 1;      /* 10^position in the decimal number               */
    idx_t t = n;          /* copy of n for manipulation                        */

    while (t > 0) {
        idx_t digit5 = t % 5;          /* digit in base 5, 0..4                 */
        seq_t digit10 = (seq_t)(2 * digit5); /* corresponding decimal digit    */
        k += digit10 * pow10;
        pow10 *= 10;
        t /= 5;
    }   /* when n==0 the loop is skipped and k stays 0 */

    /* Compute the decimal reversal of k */
    seq_t rev = 0;
    seq_t tmp = k;
    while (tmp > 0) {
        rev = rev * 10 + (tmp % 10);
        tmp /= 10;
    }

    /* Arithmetic mean */
    return (k + rev) / 2;
}
