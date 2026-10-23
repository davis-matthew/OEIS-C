/*
 * A120749  Numbers k such that {k* sqrt(2)} > 1/2, where { } = fractional
 *          part.
 * https://oeis.org/A120749
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 44, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = the n-th positive integer k such that the fractional part of k*sqrt(2) exceeds 1/2
   The test is performed by the identity
        {k*sqrt(2)} > 1/2  <=>  2*k*k - m*m > m
   where m = floor(sqrt(2*k*k)).
*/
#include "../utils/floor_sqrt.h"

static const unsigned int prefix[5] = {0,2,5,7,9};   // not used; kept for reference

seq_t a(idx_t n)
{
    idx_t count = 0;
    idx_t k = 0;
    while (count < n) {
        ++k;
        seq_t kseq = (seq_t)k;
        seq_t x = (seq_t)2 * kseq * kseq;       /* x = 2*k^2   */
        seq_t m = floor_sqrt(x);                 /* m = floor(sqrt(2*k^2))   */
        seq_t delta = x - m * m;                 /* delta = 2*k^2 - m^2   */
        if (delta > m)                           /* fractional part > 1/2  */
            ++count;
    }
    return (seq_t)k;
}
