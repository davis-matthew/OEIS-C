/*
 * A111132  a(n+1) = a(n) + (a(n) - a(n-1) + a(n) mod 10) mod 10 with a(0)=0
 *          and a(1)=1.
 * https://oeis.org/A111132
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n+1) = a(n) + (a(n) - a(n-1) + a(n) mod 10) mod 10 with a(0) =
 *            1, a(1) = 3 (offset 0 – the first term in the OEIS listing is
 *            a(0) = 1)
 * Range:     u8 n <= 52, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)3;

    seq_t a_nm2 = (seq_t)1;   /* a(0) */
    seq_t a_nm1 = (seq_t)3;   /* a(1) */
    for (idx_t i = 2; i <= n; ++i)
    {
        seq_t diff  = a_nm1 - a_nm2;
        seq_t mod10 = a_nm1 % (seq_t)10;
        seq_t inc   = (diff + mod10) % (seq_t)10;
        seq_t a_n   = a_nm1 + inc;
        a_nm2 = a_nm1;
        a_nm1 = a_n;
    }
    return a_nm1;
}
