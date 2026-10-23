/*
 * A130068  Maximal power of 2 dividing the binomial coefficient binomial(m,
 *          2^k) where m >= 1 and 1 <= 2^k <= m.
 * https://oeis.org/A130068
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = 1 + popcount(m-2^k) - popcount(m) where (m,k) are
 *            determined from the index n by n = A001855(m) + k + 1, where
 *            A001855(m) = sum_{j=1}^{m-1} (floor(log2(j))+1)
 * Range:     u8 n <= 105, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Helpers:   floor_log2_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/floor_log2_uint64.h"

/* popcount for uint64_t using only bitwise ops */
static int popcount64(uint64_t x) {
    int c = 0;
    while (x) { c++; x &= x - 1; }
    return c;
}

/* total number of terms up to and including row m (inclusive) */
static unsigned __int128 count_terms_upto(uint64_t m) {
    unsigned __int128 total = 0;
    uint64_t power = 1;
    int t = 0;
    while (power <= m) {
        uint64_t next_power = (power > UINT64_MAX >> 1) ? (uint64_t)-1 : power << 1;
        uint64_t end = (m < next_power - 1) ? m : next_power - 1;
        uint64_t cnt = end - power + 1;
        total += (unsigned __int128)(t + 1) * (unsigned __int128)cnt;
        power = next_power;
        t++;
    }
    return total;
}

/* a(n) */
seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;

    /* find smallest m such that count_terms_upto(m) >= n */
    uint64_t lo = 1, hi = 1;
    while (count_terms_upto(hi) < (unsigned __int128)n) {
        if (hi > UINT64_MAX >> 1) { hi = UINT64_MAX; break; }
        hi <<= 1;
    }
    while (lo < hi) {
        uint64_t mid = lo + ((hi - lo) >> 1);
        if (count_terms_upto(mid) >= (unsigned __int128)n)
            hi = mid;
        else
            lo = mid + 1;
    }
    uint64_t m = lo;

    /* number of terms before row m (excluding it) */
    unsigned __int128 before_m = (m == 1) ? 0 : count_terms_upto(m - 1);

    /* length of row m */
    int row_len = floor_log2_uint64(m) + 1;

    /* index offset within row */
    uint64_t k = n - (uint64_t)before_m - 1;   /* k fits in [0,row_len-1] */

    uint64_t r = 1ULL << (uint64_t)k;          /* 2^k */
    uint64_t mr = m - r;

    int pc_m  = popcount64(m);
    int pc_mr = popcount64(mr);

    int e = 1 + pc_mr - pc_m;                  /* v2(binomial(m,2^k)) */

    return (seq_t)e;
}
