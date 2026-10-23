/*
 * A123506  Sequence generated from the second nontrivial zero of the Riemann
 *          zeta function.
 * https://oeis.org/A123506
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 if the argument (in degrees) of (1/n)^z is between 0°
 *            and 180°, where z = 1/2 + i*t and t = 21.0220396387..., else
 *            a(n) = 0. The argument is computed as θ(n) = -t*ln(n) (radians),
 *            reduced modulo 2π and converted to degrees
 * Range:     u8 n <= 88, u16 n <= 88, u32 n <= 88, u64 n <= 88, u128 n <= 88,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n < 2)            /* terms below the offset are irrelevant */
        return 0;

    /* Second nontrivial zero of ζ(s) : 1/2 + i*t  */
    const double t = 21.0220396387091453581071669122406L; /* more digits for safety */

    /* natural logarithm of n (double precision is sufficient for n ≤ ~10⁴) */
    double ln_n = log((double)n);

    /* Compute the raw argument in radians: θ = -t * ln(n) */
    double ang_rad = -t * ln_n;

    /* Reduce modulo 2π (so the result lies in [0,2π)) */
    const double two_pi = 2.0 * acos(-1.0);
    double ang_norm = fmod(ang_rad, two_pi);
    if (ang_norm < 0.0)
        ang_norm += two_pi;

    /* Convert to degrees */
    double ang_deg = ang_norm * 180.0 / acos(-1.0);

    /* Check if the angle lies in the required interval.
       A tiny epsilon guards against round‑off errors at the 180° boundary. */
    const double eps = 1e-12;
    return (ang_deg <= 180.0 + eps) ? (seq_t)1 : (seq_t)0;
}
