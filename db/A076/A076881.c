/*
 * A076881  pi(n) - floor(n / log(n)), where pi(n) = the number of primes <=
 *          n.
 * https://oeis.org/A076881
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = pi(n) - floor(n / log(n))
 * Range:     i8 n <= 100, i16 n <= 100, i32 n <= 100, i64 n <= 100,
 *            i128 n <= 100, big any n
 * Helpers:   compute_pi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/compute_pi.h"
#include <math.h>
#include <stdint.h>

/* Return a(n) for n >= 2.  For n < 2 the result is unused. */
seq_t a(idx_t n)
{
    if (n < 2)
        return 0; /* offset does not apply */

    /* Number of primes ≤ n (uses the provided helper). */
    seq_t pi_val = compute_pi(n);

    /* Compute floor(n / ln(n)) using double precision.  Cast to an integer
       type that is safe for the target build (uint64_t works for all sizes
       up to the limits of the sequence). */
    double val = (double)n / log((double)n);
    uint64_t fl64 = (uint64_t)val;                /* truncation gives floor */

    /* Correct possible floating‑point error by a tiny integer adjustment. */
    double limit = val;                           /* val is exactly n/ln(n) in double */
    while ((double)(fl64 + 1) <= limit) fl64++;
    while ((double)fl64 > limit) fl64--;

    /* Convert to seq_t – this is legal for all supported seq_t types. */
    seq_t fl = (seq_t)fl64;

    return pi_val - fl;
}
