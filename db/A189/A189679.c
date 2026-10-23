/*
 * A189679  a(n) = n + floor(nr/t) + floor(ns/t) with r=1, s=arctan(1/2),
 *          t=arctan(2).
 * https://oeis.org/A189679
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n + ⌊n / arctan(2)⌋ + ⌊n * arctan(1/2) / arctan(2)⌋
 * Range:     u8 n <= 84, u16 n <= 84, u32 n <= 84, u64 n <= 84, u128 n <= 84,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* where r = 1, s = arctan(1/2), t = arctan(2) */
/* Implements a(n) for the OEIS sequence A189679 */

#include <math.h>

seq_t a(idx_t n)
{
    /* Pre‑computed values of the two irrational constants (long double). */
    const long double t = 1.107148717794090502628678190L;   /* arctan(2)            */
    const long double s = 0.4636476090008061162156369L;    /* arctan(1/2)           */

    /* Promote the idx_t argument to a native unsigned integer for
       the floating‑point computations.  This is safe because idx_t
       is uint64_t and the values of n we are interested in are much
       smaller than the range of an unsigned long long. */
    const unsigned long long u = (unsigned long long)n;

    /* Compute the two floor terms using long double arithmetic. */
    const unsigned long long term1 = (unsigned long long)floor((long double)u / t);
    const unsigned long long term2 = (unsigned long long)floor((long double)u * s / t);

    /* Convert everything to seq_t and return the sum. */
    return (seq_t)u + (seq_t)term1 + (seq_t)term2;
}
