/*
 * A125558  Central column of triangle A090181.
 * https://oeis.org/A125558
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(0)=1; for n>0, * a(n) = Catalan(n)^2*(n+1)/2 * =
 *            binom(2n,n)*binom(2n-1,n)/(n+1) * = 4*(2n-1)^2/(n*(n+1)) *
 *            a(n-1) (n>1)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 10, u64 n <= 18, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0 || n == 1)
        return 1;

    seq_t value = 1;
    for (idx_t i = 2; i <= n; ++i)
    {
        unsigned long long two_i_minus1 = 2ULL * i - 1ULL;
        unsigned long long num = 4ULL * two_i_minus1 * two_i_minus1;   /* 4*(2i-1)^2 */
        unsigned long long denom = i * (i + 1);                       /* i*(i+1) */

        if (sizeof(seq_t) <= 8)          /* use 128‑bit intermediate for 64‑bit types */
        {
            __int128 tmp = (__int128)(unsigned long long)value * (unsigned long long)num;
            value = (seq_t)(tmp / denom);
        }
        else                            /* arbitrary‑precision types handle overflow */
        {
            value = (value * (seq_t)num) / denom;
        }
    }

    return value;
}
