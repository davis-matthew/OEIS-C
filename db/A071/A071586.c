/*
 * A071586  Powers of 8 written backwards.
 * https://oeis.org/A071586
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, base
 * Formula:   a(n) = reverse( 8^n ) (decimal digit reversal)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 10, u64 n <= 21, u128 n <= 42,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* compute 8^n */
    seq_t pow = 1;
    for (idx_t i = 0; i < n; ++i)
        pow *= (seq_t)8;          /* pow = 8 * pow */

    /* reverse decimal digits of pow */
    seq_t tmp   = pow;            /* copy that we can mutate */
    seq_t rev   = 0;
    while (tmp > 0)
    {
        rev = rev * (seq_t)10 + (tmp % (seq_t)10);
        tmp = tmp / (seq_t)10;
    }
    return rev;
}
