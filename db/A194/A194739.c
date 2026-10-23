/*
 * A194739  Number of k such that {k*sqrt(3)} > {n*sqrt(3)}, where { } =
 *          fractional part.
 * https://oeis.org/A194739
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of k in 1..n such that {k*√3} > {n*√3} The
 *            function simply scans all k ≤ n, computes the fractional part of
 *            k*√3 using double arithmetic (which is safe for n ≤ 10000), and
 *            counts how many are strictly larger than that of n
 * Range:     u8 n <= 75, u16 n <= 75, u32 n <= 75, u64 n <= 75, u128 n <= 75,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>

seq_t a(idx_t n)
{
    /*  √3 constant – sufficient precision for numerators up to 10000
        (double has ≈15 decimal digits; the smallest difference between
        fractional parts for these n is ≈10⁻⁹). */
    const double SQRT3 = 1.732050807568877293527446341505872312;

    double df_n = n * SQRT3;
    double f_n  = df_n - (long long)df_n;   /* fractional part of n*√3 */

    seq_t cnt = 0;
    for (idx_t k = 1; k <= n; ++k)
    {
        double df_k = k * SQRT3;
        double f_k  = df_k - (long long)df_k;   /* fractional part of k*√3 */
        if (f_k > f_n)
            ++cnt;
    }
    return cnt;
}
