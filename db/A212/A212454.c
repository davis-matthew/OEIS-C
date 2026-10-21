/*
 * A212454  Ceiling(5n + log(5n)).
 * https://oeis.org/A212454
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ceil(5*n + log(5*n))
 * Range:     u8 n <= 49, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the n‑th term of OEIS A212454 for any n ≥ 1.  The
   implementation evaluates 5*n exactly as a `seq_t` and obtains
   floor(log(5*n)) by converting n to double (which is safe because
   n ≤ 2⁶⁴, so 5*n ≤ 9·10¹⁹ < 2⁶⁴ and fits cleanly in a double).  Since
   log(5*n) is never an integer, ceil(x) = floor(x) + 1, so the term
   can be written as 5*n + floor(log(5*n)) + 1.  */
#include <math.h>

seq_t a(idx_t n)
{
    /* Exact integer part */
    seq_t five_n = (seq_t)5 * (seq_t)n;

    /* Compute floor of log(5*n) using double precision.
       Casting `n` to double is safe because 5*n fits in a double.  */
    double ln_val = log(5.0 * (double)n);
    long long ln_floor = (long long)floor(ln_val);

    /* Since log(5*n) is never an integer, ceil(5*n + log(5*n))
       equals five_n + ln_floor + 1.  */
    return five_n + (seq_t)ln_floor + (seq_t)1;
}
