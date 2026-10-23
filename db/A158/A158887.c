/*
 * A158887  a(n) = (n+1)^n * n! * binomial(n-1 + 1/(n+1), n).
 * https://oeis.org/A158887
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Product_{k=0..n-1} (k*(n+1)+1) with a(0)=1
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 7, u64 n <= 11, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 1;
    seq_t res = 1;
    seq_t np1 = (seq_t)(n + 1);
    for (idx_t k = 0; k < n; ++k)
    {
        res *= ( (seq_t)k * np1 + 1 );
    }
    return res;
}
