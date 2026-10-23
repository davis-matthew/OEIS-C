/*
 * A034176  One third of quartic factorial numbers.
 * https://oeis.org/A034176
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(1)=1; a(n) = (4*n-1)*a(n-1) for n>=2 So a(n) = ∏_{j=1..n}
 *            (4*j-1)/3
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 14, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t res = 1;
    for (idx_t j = 2; j <= n; ++j)
    {
        res = res * ((seq_t)4 * j - 1);
    }
    return res;
}
