/*
 * A132345  Number of increasing three-term geometric sequences from the
 *          integers {1,2,...,n}.
 * https://oeis.org/A132345
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = sum_{p=2..⌊√n⌋} φ(p) * ⌊n / p²⌋
 * Range:     u8 n <= 215, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 4)                  /* for p=2 we need n≥4 */
        return (seq_t)0;

    seq_t result = (seq_t)0;

    /* integer floor sqrt of n */
    idx_t limit = 0;
    while ((limit + 1) * (limit + 1) <= n)
        ++limit;

    for (idx_t p = 2; p <= limit; ++p)
    {
        /* compute φ(p) for small p */
        idx_t phi = p;
        idx_t tmp = p;
        for (idx_t q = 2; q * q <= tmp; ++q)
        {
            if (tmp % q == 0)
            {
                phi = phi / q * (q - 1);
                while (tmp % q == 0)
                    tmp /= q;
            }
        }
        if (tmp > 1)   /* remaining prime factor */
            phi = phi / tmp * (tmp - 1);

        idx_t denom = p * p;
        idx_t t = n / denom;                /* floor(n / p²) */
        result += (seq_t)(phi * t);
    }

    return result;
}
