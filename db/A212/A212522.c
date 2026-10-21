/*
 * A212522  Number of (w,x,y,z) with all terms in {1,...,n} and w>=2x and
 *          y>3z.
 * https://oeis.org/A212522
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = m*(n-m) * (k*n - 3*k*(k+1)/2) where m = floor(n/2) and k
 *            = floor((n-1)/3)
 * Range:     u8 n <= 9, u16 n <= 36, u32 n <= 567, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This exploits the independence of the (w,x) and (y,z) constraints:
   a(n) = (#(w,x) with w>=2x) * (#(y,z) with y>3z). */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* For n < 4 there are no valid tuples; guard against underflow */
    if (n < 4)
        return 0;

    uint64_t N = (uint64_t)n;          /* use 64-bit temp to avoid overflow
                                          for n up to about 10⁴   */
    uint64_t m = N / 2;                /* m = floor(n/2)           */
    uint64_t P = m * (N - m);          /* number of (w,x) pairs     */

    uint64_t k = (N - 1) / 3;          /* k = floor((n-1)/3)       */
    uint64_t Q = k * N - (3 * k * (k + 1)) / 2; /* number of (y,z) pairs */

    uint64_t result = P * Q;           /* product gives a(n)        */
    return (seq_t)result;
}
