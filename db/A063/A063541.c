/*
 * A063541  Least number of empty triangles determined by n points in the
 *          plane.
 * https://oeis.org/A063541
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn, more
 * Formula:   a(n) = C(n-1,2) + C(n-3,2) + max(0, n-9) - floor(max(0,n-9)/4)
 * Range:     u8 n <= 13, u16 n <= 13, u32 n <= 13, u64 n <= 13, u128 n <= 13,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* C(n,k) = n*(n-1)/2 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* For n < 3 the offset is not defined; return 0 safely */
    if (n < 3)
        return 0;

    /* Work in 64‑bit unsigned to avoid intermediate overflow,
       then cast to seq_t. Natural widening is allowed in C11/C++17. */
    uint64_t N = (uint64_t)n;

    /* Base term: C(n-1,2) + C(n-3,2)  ==  n^2 - 5*n + 7 */
    uint64_t base = N * N - 5 * N + 7;

    /* Additional adjustment for n ≥ 9 */
    uint64_t k = 0;
    if (n > 9)
        k = N - 9;

    uint64_t adjust = k - (k / 4);

    uint64_t result = base + adjust;
    return (seq_t)result;
}
