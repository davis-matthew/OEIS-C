/*
 * A053417  Circle numbers (version 5): a(n) = number of points
 *          (i+j/2,j*sqrt(3)/2), i,j integers (triangular grid) contained in a
 *          circle of diameter n, centered at (1/2,0).
 * https://oeis.org/A053417
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   counts lattice points (i+j/2, j*sqrt(3)/2) within a circle of
 *            diameter n centered at (1/2,0). Equivalently, the number of
 *            integer pairs (i,j) satisfying (2i + j - 1)^2 + 3*j^2 <= n^2
 * Range:     u8 n <= 16, u16 n <= 268, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static inline uint64_t isqrt_u64(uint64_t n) {
    uint64_t low = 0, high = 1;
    while ((unsigned __int128)high * high <= n) high <<= 1;
    while (low < high) {
        uint64_t mid = (low + high + 1) >> 1;
        if ((unsigned __int128)mid * mid <= n) low = mid;
        else high = mid - 1;
    }
    return low;
}

seq_t a(idx_t n) {
    if (n == 0) return 0;
    uint64_t n2 = (uint64_t)n * (uint64_t)n;        /* n^2 */
    /* max j such that 3*j^2 <= n^2  =>  j^2 <= floor(n^2/3) */
    uint64_t limit = n2 / 3;
    int j_max = (int)isqrt_u64(limit);
    int j_min = -j_max;

    seq_t count = 0;
    for (int j = j_min; j <= j_max; ++j) {
        uint64_t jj = (uint64_t)j * (uint64_t)j;
        uint64_t rem = n2 - 3 * jj;                   /* remainder for sqrt */
        uint64_t R = isqrt_u64(rem);
        /* desired parity of t = 2i + j - 1: same as (j - 1) mod 2 */
        int offset = (j - 1) & 1;

        /* first t in [-R,R] with required parity */
        int first_t = -(int)R;
        if ( (first_t & 1) != offset ) first_t++;

        if (first_t > (int)R) continue;               /* no solutions */

        /* number of t values spaced by 2 from first_t to R */
        int span = (int)R - first_t;
        int num = span / 2 + 1;
        count += (seq_t)num;
    }
    return count;
}
