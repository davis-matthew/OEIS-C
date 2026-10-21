/*
 * A088841  Numerator of the quotient sigma(7*n)/sigma(n).
 * https://oeis.org/A088841
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, frac
 * Formula:   a(n) = (7^(v7(n)+2) - 1) / 6 , where v7(n) = exponent of 7
 *            dividing n
 * Range:     u8 n <= 48, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    /* count exponent of 7 in n */
    idx_t k = 0;
    idx_t m = n;
    while (m % 7 == 0) {
        m /= 7;
        ++k;
    }
    /* compute 7^k as seq_t */
    seq_t pow7 = 1;
    for (idx_t i = 0; i < k; ++i)
        pow7 *= (seq_t)7;
    /* a(n) = (49 * 7^k - 1) / 6 */
    return ((pow7 * 49) - 1) / 6;
}
