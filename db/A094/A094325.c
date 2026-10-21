/*
 * A094325  a(n) = 6 written in base n.
 * https://oeis.org/A094325
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 n <= 94, u64 n <= 94, u128 n <= 94,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = decimal integer consisting of the digits of 6
 * written in base n.  For n > 6 the representation is the single
 * digit 6; for n = 1 it is the unary string of six 1's (111111).
 */
seq_t a(idx_t n)
{
    /* Special case: base 1, use unary expansion */
    if (n == (idx_t)1) {
        /* 111111 = (10^6 - 1) / 9 */
        return (seq_t)111111;
    }

    /* For n >= 7, the digit 6 itself is valid, so a(n) = 6 */
    if (n >= (idx_t)7) {
        return (seq_t)6;
    }

    /* For 2 <= n <= 6, compute base-n representation of 6
       and interpret it as a decimal integer.  The representation
       will have at most 3 digits (for n=2). */
    idx_t tmp = (idx_t)6;
    int digits[10];
    int dcnt = 0;

    while (tmp > (idx_t)0) {
        digits[dcnt++] = (int)(tmp % n);
        tmp /= n;
    }

    seq_t result = (seq_t)0;
    while (dcnt-- > 0) {
        result = result * (seq_t)10 + (seq_t)(digits[dcnt]);
    }

    return result;
}
