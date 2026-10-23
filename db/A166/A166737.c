/*
 * A166737  Number of primes in (n^2*log(n)..(n+1)^2*log(n+1)] semi-open
 *          intervals, n >= 1.
 * https://oeis.org/A166737
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = pi((n+1)^2 * log(n+1)) - pi(floor(n^2 * log(n)))
 * Range:     u8 n <= 141, u16 n <= 141, u32 n <= 141, u64 n <= 141,
 *            u128 n <= 141, big any n
 * Helpers:   compute_pi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>
#include "../utils/compute_pi.h"

seq_t a(idx_t n)
{
    /* The sequence is defined for n >= 1.  For n == 0 return 0. */
    if (n == 0) {
        return 0;
    }

    /* Compute the logarithm bounds in double precision. */
    double dn   = (double) n;
    double dnp1 = dn + 1.0;

    double lower = dn * dn * log(dn);
    double upper = dnp1 * dnp1 * log(dnp1);

    /* Convert to integer indices by truncation (floor). */
    idx_t lower_int = (idx_t)floor(lower);
    idx_t upper_int = (idx_t)floor(upper);

    /* Use the supplied compute_pi routine to get the prime counts. */
    seq_t pi_upper = compute_pi(upper_int);
    seq_t pi_lower = compute_pi(lower_int);

    /* The interval is semi‑open: (lower, upper] */
    return pi_upper - pi_lower;
}
