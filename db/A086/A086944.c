/*
 * A086944  a(n) = k where R(k+8) = 5.
 * https://oeis.org/A086944
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = 5 * 10^n - 8 (offset = 1)
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 8, u64 n <= 18, u128 n <= 37,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Compute 10^n in type seq_t. */
    seq_t pow10 = (seq_t)1;
    for (idx_t i = 0; i < n; ++i)
        pow10 *= (seq_t)10;

    /* a(n) = 5*10^n - 8.  All constants are cast to seq_t to avoid
       accidental promotion or overflow in intermediate arithmetic. */
    return pow10 * (seq_t)5 - (seq_t)8;
}
