/*
 * A053621  Nearest integer to n/(log(n)-1).
 * https://oeis.org/A053621
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = nearest integer to n/(log(n)-1)
 * Range:     i8 n <= 709, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>

seq_t a(idx_t n)
{
    /* The sequence is defined for n ≥ 1.  For n = 0 we return 0
       as a harmless placeholder (offset 1 in OEIS). */
    if (n == 0)
        return 0;

    long double nd = (long double)n;
    long double denom = log(nd) - 1.0L;        /* log(n) – 1   */
    long double val  = nd / denom;            /* n / (log(n)-1) */

    /* Round to the nearest integer.  llround rounds half-way
       away from zero, which matches the OEIS “nearest integer”
       convention for the presented data. */
    long long rounded = llround(val);

    return (seq_t)rounded;
}
