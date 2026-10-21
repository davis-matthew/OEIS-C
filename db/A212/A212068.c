/*
 * A212068  Number of (w,x,y,z) with all terms in {1,...,n} and 2w=x+y+z.
 * https://oeis.org/A212068
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(10*n^2 - 3*n + 2)/24 , n even (n-1)*(10*n^2 + 7*n +
 *            9)/24 , n odd
 * Range:     u8 n <= 8, u16 n <= 54, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Use a 64‑bit intermediate to avoid overflow of the intermediate
       products for the range of n that will be tested. */
    uint64_t N = (uint64_t)n;

    if ((N & 1ULL) == 0ULL) {                /* n even   */
        uint64_t num = N * ((uint64_t)10 * N * N
                           - (uint64_t)3 * N + 2ULL);
        return (seq_t)(num / 24ULL);
    } else {                                 /* n odd    */
        uint64_t num = (N - 1ULL) * ((uint64_t)10 * N * N
                                    + (uint64_t)7 * N + 9ULL);
        return (seq_t)(num / 24ULL);
    }
}
