/*
 * A162626  If 0 <= n <= 3 then a(n) = n(n+1)(n+2)/3, if n >= 4 then a(n) =
 *          n(n^2+5)/3.
 * https://oeis.org/A162626
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = n*(n+1)*(n+2)/3 for 0 <= n <= 3, a(n) = n*(n*n + 5)/3 for
 *            n >= 4
 * Range:     u8 n <= 8, u16 n <= 43, u32 n <= 43, u64 n <= 43, u128 n <= 43,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t N = (seq_t)n;
    if (n < 4)
        return N * (N + 1) * (N + 2) / 3;
    else
        return N * (N * N + 5) / 3;
}
