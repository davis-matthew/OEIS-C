/*
 * A128814  a(0)=1, a(n)= Product_{k=1..n} k*(k+1)/2+1.
 * https://oeis.org/A128814
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = PROD_{k=1..n} (k*(k+1)/2 + 1)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 13, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t res = (seq_t)1;
    for (idx_t k = (idx_t)1; k <= n; ++k)
    {
        seq_t mk = (seq_t)k;
        seq_t term = mk * (mk + (seq_t)1) / (seq_t)2 + (seq_t)1;
        res *= term;
    }
    return res;
}
