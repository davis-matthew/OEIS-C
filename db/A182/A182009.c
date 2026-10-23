/*
 * A182009  a(n) = ceiling(sqrt(2n*log(2))+(3-2*log(2))/6).
 * https://oeis.org/A182009
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ceil(sqrt(2*n*log(2)) + (3-2*log(2))/6)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;

    /* Constants for scaled arithmetic */
    const idx_t D   = 1000000ULL;          /* scaling factor for log(2)      */
    const idx_t L   = 693147ULL;           /* floor(log(2) * D)              */
    const idx_t Cnum = 3 * D - 2 * L;      /* numerator of the additive part */
    const idx_t Cden = 6 * D;              /* denominator of the additive part */

    /* RHS of the inequality: 72 * n * L * D   (fits in 64‑bit for n ≤ 10⁴) */
    const unsigned long long rhs = 72ULL * n * L * D;

    for (idx_t m = 0; ; ++m)
    {
        /* Compute (m*Cden - Cnum)^2 in unsigned 64‑bit. */
        long long diff = (long long)m * (long long)Cden - (long long)Cnum;
        unsigned long long diff_abs = diff >= 0 ? (unsigned long long)diff
                                                : (unsigned long long)(-diff);
        unsigned long long lhs = diff_abs * diff_abs;

        if (lhs >= rhs)
            return (seq_t)m;
    }
}
