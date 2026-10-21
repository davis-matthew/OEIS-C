/*
 * A053415  Circle numbers (version 3): a(n) = number of points (i,j), i,j
 *          integers, contained in a circle of diameter n, centered at (1/2,
 *          1/2).
 * https://oeis.org/A053415
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = |{ (i,j) ∈ ℤ² : (i-½)² + (j-½)² ≤ (n/2)² }| * Equivalent:
 *            count odd integers u,v with u² + v² ≤ n², u = 2i-1, v = 2j-1. *
 *            We iterate over positive odd u, compute maximal odd |v|, and sum
 *            the * contributions. Complexity O(n)
 * Range:     u8 n <= 17, u16 n <= 288, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static uint64_t isqrt_uint128(unsigned __int128 x)
{
    /* binary search for floor(sqrt(x)), 0 ≤ x < 2^128 */
    unsigned __int128 low = 0, high = (unsigned __int128)1 << 64; /* sqrt(2^128-1) < 2^64 */
    while (low < high) {
        unsigned __int128 mid = (low + high + 1) >> 1;
        if (mid * mid <= x)
            low = mid;
        else
            high = mid - 1;
    }
    return (uint64_t)low;
}

seq_t a(idx_t n)
{
    seq_t count = 0;
    unsigned __int128 n128 = (unsigned __int128)n;
    unsigned __int128 n2   = n128 * n128; /* n² */

    for (uint64_t u = 1; u <= n; u += 2) {          /* positive odd u */
        unsigned __int128 rem = n2 - (unsigned __int128)u * u;  /* n² - u² ≥ 0 */
        uint64_t M = isqrt_uint128(rem);                        /* floor sqrt(rem) */
        seq_t cntPosOddV = (seq_t)((M + 1) / 2);                /* number of positive odd v */
        count += (seq_t)4 * cntPosOddV;                         /* ±u and ±v symmetry */
    }
    return count;
}
