/*
 * A083318  a(0) = 1; for n>0, a(n) = 2^n + 1.
 * https://oeis.org/A083318
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2^n + 1 for n > 0, a(0) = 1
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 31, u64 n <= 63, u128 n <= 127,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* For n == 0 the value is defined explicitly. */
    if (n == 0)
        return (seq_t)1;

    /* Compute 2^n by repeated multiplication.  For n larger than fits
       into the bit-width of seq_t the behavior may overflow, but the
       sequence is only tested for values that fit in the underlying
       type. */
    seq_t result = (seq_t)1;
    for (idx_t i = 0; i < n; ++i)
        result = (seq_t)((seq_t)result * ((seq_t)2));

    /* Add the constant term (+1) and return. */
    return result + (seq_t)1;
}
