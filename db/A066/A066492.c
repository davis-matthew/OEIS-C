/*
 * A066492  a(n) = A056524(n)/11.
 * https://oeis.org/A066492
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = (n*10^digits(n) + reverse(n))/11
 * Range:     u8 n <= 9, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the n-th term of OEIS A066492: the quotient of the concatenation of n with its reverse, divided by 11. */

seq_t a(idx_t n)
{
    /* convert index to seq_t for arithmetic */
    seq_t nn = (seq_t)n;

    /* determine number of decimal digits of n */
    idx_t tmp = n;
    idx_t d  = 0;
    do {
        ++d;
        tmp /= 10;
    } while (tmp > 0);

    /* compute 10^d as seq_t */
    seq_t pow10 = 1;
    for (idx_t i = 0; i < d; ++i)
        pow10 *= 10;

    /* compute reverse of n */
    seq_t rev = 0;
    tmp = n;
    while (tmp > 0) {
        seq_t digit = tmp % 10;
        rev = rev * 10 + digit;
        tmp /= 10;
    }

    /* concatenate n and reverse(n), then divide by 11 */
    seq_t val = nn * pow10 + rev;
    return val / 11;
}
