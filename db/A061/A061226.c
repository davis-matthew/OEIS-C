/*
 * A061226  a(n) = n^2 + (n^2 with digits reversed).
 * https://oeis.org/A061226
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy, look
 * Formula:   a(n) = n^2 + rev(n^2) (rev = decimal digit reversal)
 * Range:     u8 n <= 11, u16 n <= 102, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    /* compute n^2 */
    seq_t sq = (seq_t)n * ((seq_t)n);

    /* reverse decimal digits of sq */
    seq_t rev = (seq_t)0;
    seq_t tmp = sq;
    while (tmp != (seq_t)0) {
        seq_t digit = tmp % (seq_t)10;   /* least significant decimal digit */
        rev = rev * (seq_t)10 + digit;   /* append digit to reversed number */
        tmp = tmp / (seq_t)10;           /* drop processed digit */
    }

    /* return sum of square and its decimal reverse */
    return sq + rev;
}
