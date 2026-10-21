/*
 * A067082  If n = abc...def in decimal notation then the right digit sum
 *          function = abc...def + bc...def + c...def + ... + def + ef + f.
 * https://oeis.org/A067082
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, nonn, easy
 * Formula:   a(n) = sum_{k=1}^{len(n)} n mod 10^k (right digit sum: n +
 *            suffix without first digit + suffix without first two digits + …
 *            + last digit) Example: a(256) = 256 + 56 + 6 = 318
 * Range:     u8 n <= 175, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    /* Handle n = 0 explicitly */
    if (n == 0) {
        return 0;
    }

    /* Compute number of decimal digits */
    idx_t tmp = n;
    unsigned int len = 0;
    while (tmp > 0) {
        ++len;
        tmp /= 10;
    }

    seq_t result   = 0;
    seq_t nseq     = (seq_t)n;
    seq_t pow10    = 10;            /* 10^1 */

    for (unsigned int i = 1; i <= len; ++i) {
        /* Add suffix of length i (last i digits) */
        result += nseq % pow10;
        /* Avoid overflow in the last step */
        if (i != len) {
            pow10 *= 10;
        }
    }

    return result;
}
