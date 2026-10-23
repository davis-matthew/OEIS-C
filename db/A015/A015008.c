/*
 * A015008  q-factorial numbers for q=9.
 * https://oeis.org/A015008
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Product_{k=1..n} (9^k - 1)/8 Recurrence: a(0)=1 a(n) =
 *            (9^n - 1)*a(n-1)/8 (n>=1)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 4, u64 n <= 6, u128 n <= 9,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t result = (seq_t)1;
    seq_t pow9   = (seq_t)1;
    for (idx_t i = 1; i <= n; ++i)
    {
        pow9 *= (seq_t)9;
        seq_t factor = (pow9 - (seq_t)1) / (seq_t)8;
        result *= factor;
    }
    return result;
}
