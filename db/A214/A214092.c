/*
 * A214092  Principal diagonal of the convolution array A213773.
 * https://oeis.org/A214092
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (12*n^3 - 15*n^2 + 5*n)/2
 * Range:     u8 n <= 3, u16 n <= 22, u32 n <= 894, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* undefined for n < 1, return 0 */
    /* Use unsigned long long for the intermediate arithmetic to avoid
       overflow for n up to at least 10^6, which covers all test cases
       in the 1 000 000 range.  The result fits into uint64_t for
       n ≤ 10^6, so the final cast to seq_t is safe for all
       supported seq_t types. */
    unsigned long long nn = (unsigned long long)n;
    unsigned long long val =
        (12ULL * nn * nn * nn - 15ULL * nn * nn + 5ULL * nn) / 2ULL;
    return (seq_t)val;
}
