/*
 * A120777  a(n) = 2^(2*n - valuation(CatalanNumber(n), 2)).
 * https://oeis.org/A120777
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, frac
 * Formula:   a(n) = 2^(2*n - valuation(CatalanNumber(n),2))
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 32, u128 n <= 64,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation: e = 2*n - popcount(n) + v2(n+1), then return 2^e. */
#include "../utils/pow2.h"

static inline int popcount_u64(idx_t x)
{
    int cnt = 0;
    while (x) {
        cnt++;
        x &= x - 1;           /* remove lowest set bit */
    }
    return cnt;
}

static inline int tz_u64(idx_t x)
{
    int cnt = 0;
    while ((x & 1ULL) == 0) {
        cnt++;
        x >>= 1;
    }
    return cnt;
}

seq_t a(idx_t n)
{
    int pop = popcount_u64(n);
    int tz  = tz_u64(n + 1);
    idx_t e = 2ULL * n - (idx_t)pop + (idx_t)tz;
    return pow2(e);
}
