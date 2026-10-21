/*
 * A052737  a(n) = ((2*n)!/n!)*2^(2*n+1).
 * https://oeis.org/A052737
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(0)=0; a(1)=2; a(n+1) = (16*n - 8)*a(n) for n≥1
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 6, u64 n <= 11, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;
    seq_t result = (seq_t)2;
    idx_t i;
    for (i = 1; i < n; ++i)
    {
        seq_t factor = (seq_t)(16 * i - 8);
        result = factor * result;
    }
    return result;
}
