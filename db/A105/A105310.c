/*
 * A105310  a(n) = concatenation of i n times, with i = 1 to n.
 * https://oeis.org/A105310
 *
 * Offset:    21    (a(21) is the first term)
 * Keywords:  base, easy, nonn, less
 * Formula:   a(n) = concat_{i=1}^{n-20} i repeated (n-20) times, n>=21 (ie
 *            a(22)=1122, a(23)=111222333, a(24)=1111222233334444, ...)
 * Range:     u8 n <= 21, u16 n <= 22, u32 n <= 23, u64 n <= 24, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 21)
        return 0;              /* undefined below offset; arbitrary value */

    /* m = n - 20 gives the repetition count */
    idx_t m = n - 20;

    seq_t result = 0;

    /* pre‑compute 10^d for d = 0..12 (enough for n up to ~10000) */
    seq_t pow10[20];
    pow10[0] = 1;
    for (int d = 1; d < 20; ++d)
        pow10[d] = pow10[d - 1] * 10;

    /* build the concatenation */
    for (idx_t i = 1; i <= m; ++i)
    {
        /* find digit length d of i */
        int d = 1;
        idx_t tmp = i;
        while (tmp >= 10)
        {
            tmp /= 10;
            d++;
        }

        seq_t mult = pow10[d];          /* 10^d */

        /* append i, m times */
        for (idx_t t = 0; t < m; ++t)
        {
            result = result * mult + i;
        }
    }

    return result;
}
