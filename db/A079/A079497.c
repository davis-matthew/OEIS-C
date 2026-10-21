/*
 * A079497  a(1)=1; for n > 2, a(n) is the smallest integer > a(n-1) such that
 *          frac(sqrt(5)*a(n)) < frac(sqrt(5)*a(n-1)).
 * https://oeis.org/A079497
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(1)=1, a(2)=5, a(3)=9, a(4)=13, a(5)=17, a(6)=89, a(7)=161,
 *            a(8)=233; for n>8, a(n) = 18*a(n-4) - a(n-8)
 * Range:     u8 n <= 8, u16 n <= 15, u32 n <= 30, u64 n <= 61, u128 n <= 122,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)5;
    if (n == 3) return (seq_t)9;
    if (n == 4) return (seq_t)13;
    if (n == 5) return (seq_t)17;
    if (n == 6) return (seq_t)89;
    if (n == 7) return (seq_t)161;
    if (n == 8) return (seq_t)233;

    seq_t buf[8];
    buf[0] = (seq_t)1;
    buf[1] = (seq_t)5;
    buf[2] = (seq_t)9;
    buf[3] = (seq_t)13;
    buf[4] = (seq_t)17;
    buf[5] = (seq_t)89;
    buf[6] = (seq_t)161;
    buf[7] = (seq_t)233;

    seq_t res = 0;
    for (idx_t i = 9; i <= n; ++i)
    {
        seq_t term_n_minus_4 = buf[(i - 5) & 7];   /* a(i-4) */
        seq_t term_n_minus_8 = buf[(i - 9) & 7];   /* a(i-8) */
        res = (seq_t)((seq_t)18 * term_n_minus_4 - term_n_minus_8);
        buf[(i - 1) & 7] = res;
    }
    return res;
}
