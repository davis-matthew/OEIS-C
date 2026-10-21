/*
 * A083583  a(n) = (8*3^n - 5*0^n)/3.
 * https://oeis.org/A083583
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (8*3^n - 5*0^n)/3 (a(0)=1, a(n)=8*3^(n-1) for n>0)
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 19, u64 n <= 39, u128 n <= 79,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Special case for n = 0 */
    if (n == 0)
        return (seq_t)1;

    /* Compute 3^(n-1) and multiply by 8 */
    seq_t result = (seq_t)1;
    for (idx_t i = 1; i < n; ++i)
        result *= (seq_t)3;    /* result = 3^(n-1) */

    result *= (seq_t)8;        /* result = 8 * 3^(n-1) */
    return result;
}
