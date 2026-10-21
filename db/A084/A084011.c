/*
 * A084011  Digit reversal of 11*n, divided by 11.
 * https://oeis.org/A084011
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = rev(11*n) / 11
 * Range:     u8 n <= 12, u16 n <= 5318, u32 n <= 9999, u64 n <= 9999,
 *            u128 n <= 9999, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* rev(x) = decimal digit reversal of x       */

seq_t a(idx_t n)
{
    seq_t x   = (seq_t)n * 11;   /* 11 * n  */
    seq_t rev = 0;

    /* reverse the decimal digits of x */
    while (x != 0) {
        rev = rev * 10 + (x % 10);
        x   = x / 10;
    }

    /* division by 11 yields the required term */
    return rev / 11;
}
