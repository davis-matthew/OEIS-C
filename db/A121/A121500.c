/*
 * A121500  Minimal polygon values for a certain polygon problem leading to an
 *          approximation of Pi.
 * https://oeis.org/A121500
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = argmin_{m≥3} |(Fin(n)+Fout(m))/2 - π|, where Fin(n) =
 *            (n/2) * sin(2π/n) and Fout(m) = m * tan(π/m)
 * Range:     u8 n <= 60, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>

seq_t a(idx_t n)
{
    /* Undefined for n < 3; just return 0 for safety. */
    if (n < 3) return 0;

    /* Precompute constants. */
    const double pi = acos(-1.0);
    const double two_pi_over_n = 2.0 * pi / (double)n;
    const double fin = (n / 2.0) * sin(two_pi_over_n);

    /* Search up to n+500 (at least 500). */
    idx_t mmax = (n > 500) ? n + 500 : 500;

    double best_diff = 1e308;   /* large number */
    idx_t best_m = 3;

    for (idx_t m = 3; m <= mmax; ++m) {
        double fout = m * tan(pi / (double)m);
        double avg = (fin + fout) * 0.5;
        double diff = fabs(avg - pi);
        if (diff < best_diff) {
            best_diff = diff;
            best_m = m;
        }
    }

    return (seq_t)best_m;
}
