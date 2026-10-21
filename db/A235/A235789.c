/*
 * A235789  Expansion of x * (1 - 2*x + 8*x^5 - 8*x^6) / (1 - 4*x^4)^2.
 * https://oeis.org/A235789
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(4*n) = 0 a(4*n+1) = (n+1) * 2^(2*n) a(4*n+2) = -2^(2*n+1)
 *            a(4*n+3) = -(n ) * 2^(2*n+1) (for all n ≥ 0)
 * Range:     i8 n <= 12, i16 n <= 26, i32 n <= 56, i64 n <= 118,
 *            i128 n <= 244, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static seq_t pow2_exp(idx_t exp)
{
    seq_t result = 1;          /* 2^0 */
    seq_t base   = 2;          /* 2^1 */

    while (exp)
    {
        if (exp & 1u)
            result *= base;
        exp >>= 1;
        if (exp)
            base *= base;
    }
    return result;
}

seq_t a(idx_t n)
{
    const idx_t k = n >> 2;        /* n / 4 */
    const idx_t r = n & 3;        /* n % 4 */

    switch (r)
    {
        case 0u:
            return (seq_t)0;

        case 1u:
            /* a(4*k+1) = (k+1) * 2^(2*k) */
            return (seq_t)(k + 1) * pow2_exp(2u * k);

        case 2u:
            /* a(4*k+2) = -2^(2*k+1) */
            return -(pow2_exp(2u * k + 1u));

        case 3u:
            /* a(4*k+3) = -(k) * 2^(2*k+1) */
            return -( (seq_t)k * pow2_exp(2u * k + 1u) );

        default:
            return (seq_t)0;  /* unreachable */
    }
}
