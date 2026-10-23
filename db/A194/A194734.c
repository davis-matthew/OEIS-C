/*
 * A194734  Number of k such that {-k*r} > {-n*r}, where { } = fractional
 *          part, r = (1+sqrt(5))/2 (the golden ratio).
 * https://oeis.org/A194734
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of k (1 ≤ k < n) with {-k*r} > {-n*r}, where r =
 *            (1+√5)/2. Since {-x} = -{x} for x>0, this is equivalent to
 *            counting k with {k*r} < {n*r}
 * Range:     u8 n <= 77, u16 n <= 77, u32 n <= 77, u64 n <= 77, u128 n <= 77,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n <= 1)
        return 0;

    /* Golden ratio as a long double constant. */
    static const long double phi = 1.6180339887498948482045868343656381177203L;

    /* Fractional part of n*phi. */
    long double nphi = (long double)n * phi;
    long long intpart_n = (long long)nphi;          /* truncates toward zero, same as floor for positives */
    long double frac_n = nphi - (long double)intpart_n;

    seq_t cnt = 0;
    for (idx_t k = 1; k < n; ++k) {
        long double kphi = (long double)k * phi;
        long long intpart_k = (long long)kphi;
        long double frac_k = kphi - (long double)intpart_k;
        if (frac_k < frac_n)
            ++cnt;
    }
    return cnt;
}
