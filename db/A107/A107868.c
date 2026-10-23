/*
 * A107868  Column 0 of triangle A107867; a(n) = C( n*(n-1)/2 + n + 1, n).
 * https://oeis.org/A107868
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = C( n*(n+1)/2 + 1, n )
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 15, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute the greatest common divisor of two values of type seq_t. */
static inline seq_t gcd_seq(seq_t a, seq_t b)
{
    while (b)
    {
        seq_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

seq_t a(idx_t n)
{
    /* N = n*(n+1)/2 + 1 */
    seq_t N = ((seq_t)n * ((seq_t)n + 1) / 2) + 1;
    idx_t k = n;

    seq_t res = 1;
    for (idx_t i = 1; i <= k; ++i)
    {
        seq_t num   = N - k + i;          /* numerator of the i‑th factor */
        seq_t denom = (seq_t)i;           /* denominator of the i‑th factor */

        /* cancel common factors between numerator and denominator */
        seq_t g = gcd_seq(num, denom);
        if (g > 1)
        {
            num   /= g;
            denom /= g;
        }

        /* cancel common factors between the current result and the denominator */
        g = gcd_seq(res, denom);
        if (g > 1)
        {
            res   /= g;
            denom /= g;
        }

        /* multiply and divide */
        res = res * num;
        if (denom != 1)
            res = res / denom;   /* should not happen except for safety */
    }

    return res;
}
