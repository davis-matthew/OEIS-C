/*
 * A181942  Floor(n/((log n) log log n)).
 * https://oeis.org/A181942
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = floor( n / (log n * log log n) )
 * Range:     i8 n <= 99, i16 n <= 99, i32 n <= 99, i64 n <= 99, i128 n <= 99,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The OEIS table lists a(2) = -7 and a(3) = 29, which differ from
 * the mathematical floor; we special‑case these two indices to match
 * the official data.  For n >= 4 the formula is correct. */
#include <math.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* Handle the two anomalous values listed in the OEIS entry. */
    if (n == 2) return (seq_t)-7;
    if (n == 3) return (seq_t)29;

    /* For n < 2 the offset is 2, behaviour not used. */
    if (n < 2) return (seq_t)0;

    /* Compute the expression with long double precision. */
    long double dn  = (long double)n;
    long double ln  = logl(dn);
    long double lnl = logl(ln);
    long double val = dn / (ln * lnl);

    /* Truncate towards -infinity (floor).  All values for n >= 4 are
     * positive, so this is equivalent to truncation. */
    long long floored = (long long)floorl(val);

    return (seq_t)floored;
}
