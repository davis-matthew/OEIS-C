/*
 * A020335  Numbers whose base-7 representation is the juxtaposition of two
 *          identical strings.
 * https://oeis.org/A020335
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = n*7^floor(log_7(n)+1) + n
 * Range:     u8 n <= 6, u16 n <= 190, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* number of digits of n in base 7 */
    idx_t t = n;
    idx_t digits = 0;
    do {
        digits++;
        t /= 7;
    } while (t);

    /* compute 7^digits as seq_t */
    seq_t pow = (seq_t)1;
    for (idx_t i = 0; i < digits; ++i) {
        pow *= (seq_t)7;
    }

    /* a(n) = n * pow + n */
    return ((seq_t)n * pow) + (seq_t)n;
}
