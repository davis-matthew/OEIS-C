/*
 * A189796  n+[ns/r]+[nt/r]; r=2, s=(sin(1))^2, t=(cos(1))^2.
 * https://oeis.org/A189796
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n + floor(n*sin(1)^2 / 2) + floor(n*cos(1)^2 / 2)
 * Range:     u8 n <= 85, u16 n <= 85, u32 n <= 85, u64 n <= 85, u128 n <= 85,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* High‑precision constants for sin^2(1) and cos^2(1).  */
static const long double S = 0.7080734182735714392210234546010830L; /* sin(1)^2 */
static const long double T = 0.29192658172642856077897654539891696L;/* cos(1)^2 */

/* Small epsilon to guard against binary rounding errors when casting to integer. */
static const long double EPS = 1e-12L;

seq_t a(idx_t n)
{
    /* Base value n. */
    seq_t res = n;

    /* Compute the two floor terms. Convert the floating value to
     * an unsigned 64‑bit integer before adding it to `res`. */
    long double v1 = ((long double)n) * S / 2.0L;
    long double v2 = ((long double)n) * T / 2.0L;

    res += (seq_t)((unsigned long long)(v1 + EPS));
    res += (seq_t)((unsigned long long)(v2 + EPS));

    return res;
}
