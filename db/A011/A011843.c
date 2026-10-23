/*
 * A011843  a(n) = floor(binomial(n,5)/6).
 * https://oeis.org/A011843
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor(binomial(n,5)/6)
 * Range:     u8 n <= 5, u16 n <= 31, u32 n <= 316, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static inline seq_t _gcd_seq(seq_t a, seq_t b)
{
    while (b != (seq_t)0)
    {
        seq_t r = a % b;
        a = b;
        b = r;
    }
    return a;
}

seq_t a(idx_t n)
{
    if (n < (idx_t)5)
        return (seq_t)0;

    seq_t den = (seq_t)720;   /* 5! * 6 = 720 */
    seq_t res = (seq_t)1;

    /* Multiply by n, n-1, n-2, n-3, n-4, reducing the denominator each step */
    for (idx_t i = (idx_t)0; i < (idx_t)5; ++i)
    {
        seq_t term = (seq_t)(n - i);
        seq_t g = _gcd_seq(term, den);
        term /= g;
        den /= g;
        res *= term;
    }

    /* Should have den == 1, but keep division for safety */
    if (den != (seq_t)1)
        res /= den;

    return res;
}
