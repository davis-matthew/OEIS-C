/*
 * A161793  If b(n) is the number of 0's in the binary representation of n,
 *          then the positive integer n is included if n = k^b(n), for some k
 *          = integer.
 * https://oeis.org/A161793
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = the nth positive integer that satisfies n = k^b(n) for
 *            some integer k, where b(n) is the number of zeros in the binary
 *            representation of n
 * Range:     u8 n <= 23, u16 n <= 58, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Helpers:   pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <math.h>
#include <limits.h>
#include "../utils/pow_uint.h"  /* big_pow alternative */

static uint64_t kth_root_uint64(uint64_t n, uint64_t k)
{
    if (k == 1) return n;
    uint64_t low = 1, high;
    long double approx = pow((long double)n, 1.0L / (long double)k);
    high = (uint64_t)approx + 2;
    if (high > n) high = n;
    while (low < high) {
        uint64_t mid = (low + high + 1) >> 1;
        seq_t val = pow_uint((uint64_t)mid, k);
        if (val <= (seq_t)n) low = mid;
        else high = mid - 1;
    }
    return low;
}

seq_t a(idx_t target)
{
    idx_t found = 0;
    uint64_t cur = 0;
    while (found < target) {
        cur++;
        uint64_t bits = 64 - __builtin_clzll(cur);
        uint64_t ones = __builtin_popcountll(cur);
        uint64_t zeros = bits - ones;
        if (zeros == 0) {
            if (cur == 1) found++;
            continue;
        }
        if (zeros == 1) {
            found++;
            continue;
        }
        uint64_t root = kth_root_uint64(cur, zeros);
        seq_t v = pow_uint(root, zeros);
        if (v == (seq_t)cur) {
            found++;
            continue;
        }
        seq_t v2 = pow_uint(root + 1, zeros);
        if (v2 == (seq_t)cur) {
            found++;
            continue;
        }
    }
    return (seq_t)cur;
}
