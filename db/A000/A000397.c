/*
 * A000397  Number of partitions into non-integral powers.
 * https://oeis.org/A000397
 *
 * Offset:    5    (a(5) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = #{(x1,x2,x3): 1<=x1<x2<x3, sqrt(x1)+sqrt(x2)+sqrt(x3) <=
 *            n}. Counted by brute force: loop over x1, then a two-pointer
 *            sweep over x2 (going up) and the largest allowed x3 (going
 *            down). Each test is exact, using integer squaring in 128-bit
 *            arithmetic
 * Range:     u8 n <= 7, u16 n <= 18, u32 n <= 80, u64 n <= 80, u128 n <= 80,
 *            big any n
 * Validated: partial, pass, 76 terms through n = 80 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <math.h>

/* exact test: sqrt(x)+sqrt(y)+sqrt(z) <= N, using integers only */
static int le_exact(uint64_t x, uint64_t y, uint64_t z, uint64_t N)
{
    __int128 N2 = (__int128)N * N;
    if ((__int128)z > N2) return 0;
    /* sqrt(x)+sqrt(y) <= N - sqrt(z)  <=>  2sqrt(xy) + 2N sqrt(z) <= R */
    __int128 R = N2 + (__int128)z - (__int128)x - (__int128)y;
    if (R < 0) return 0;
    /* square: 8N sqrt(xyz) <= S */
    __int128 S = R * R - 4 * (__int128)x * (__int128)y - 4 * N2 * (__int128)z;
    if (S < 0) return 0;
    unsigned __int128 lhs = (unsigned __int128)64 * (unsigned __int128)N2
                          * (unsigned __int128)x * (unsigned __int128)y
                          * (unsigned __int128)z;
    unsigned __int128 rhs = (unsigned __int128)S * (unsigned __int128)S;
    return lhs <= rhs;
}

static int le(uint64_t x, uint64_t y, uint64_t z, uint64_t N)
{
    double d = sqrt((double)x) + sqrt((double)y) + sqrt((double)z) - (double)N;
    if (d < -1e-6) return 1;
    if (d > 1e-6) return 0;
    return le_exact(x, y, z, N);
}

seq_t a(idx_t n)
{
    seq_t total = 0;
    uint64_t N = (uint64_t)n;
    uint64_t x;
    for (x = 1; le(x, x + 1, x + 2, N); x++) {
        uint64_t y = x + 1;
        double t = (double)N - sqrt((double)x) - sqrt((double)y);
        uint64_t c = (uint64_t)(t * t) + 2;
        uint64_t cnt = 0;
        for (;;) {
            while (c > y && !le(x, y, c, N)) c--;
            if (c <= y) break;
            cnt += c - y;
            y++;
        }
        total += (seq_t)cnt;
    }
    return total;
}
