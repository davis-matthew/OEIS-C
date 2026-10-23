/*
 * A119785  Numerator of the product of the n-th square pyramidal number and
 *          the n-th generalized harmonic number in power 2.
 * https://oeis.org/A119785
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  frac, nonn
 * Formula:   a(n) = numerator[ n*(n+1)*(2n+1)/6 * Σ_{j=1}^n 1/j^2 ]
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 19, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static inline unsigned __int128 gcd128(unsigned __int128 a, unsigned __int128 b)
{
    while (b != 0)
    {
        unsigned __int128 r = a % b;
        a = b;
        b = r;
    }
    return a;
}

seq_t a(idx_t n)
{
    /* Square‑pyramidal number S1 as 128‑bit integer */
    unsigned __int128 nn = n;
    unsigned __int128 S1 = nn * (nn + 1) * (2 * nn + 1) / 6;

    /* Cached H2 fraction cur_num / cur_den, zero‑indexed */
    static unsigned __int128 cur_num = 0;
    static unsigned __int128 cur_den = 1;
    static idx_t cached = 0;

    if (n > cached)
    {
        for (idx_t j = cached + 1; j <= n; ++j)
        {
            unsigned __int128 j2 = (unsigned __int128)j * j;
            unsigned __int128 new_num = cur_num * j2 + cur_den;
            unsigned __int128 new_den = cur_den * j2;
            unsigned __int128 g = gcd128(new_num, new_den);
            cur_num = new_num / g;
            cur_den = new_den / g;
            cached = j;
        }
    }

    /* Reduce S1 with the current denominator first */
    unsigned __int128 g1 = gcd128(S1, cur_den);
    unsigned __int128 S1_red = S1 / g1;
    unsigned __int128 den_red = cur_den / g1;

    /* Reduce the numerator with the reduced denominator */
    unsigned __int128 g2 = gcd128(cur_num, den_red);
    unsigned __int128 num_red = cur_num / g2;

    /* Final numerator */
    unsigned __int128 result = S1_red * num_red;
    return (seq_t)result;
}
