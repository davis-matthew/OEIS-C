/*
 * A071913  Smallest k such that A069624(k) = n.
 * https://oeis.org/A071913
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = smallest k such that the number of distinct values of
 *            floor(k^(1/j)), j>=2, equals n; found by doubling then binary
 *            search on k (assumes that count is nondecreasing in k)
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 15, u64 n <= 23, u128 n <= 23,
 *            big any n
 * Validated: partial, pass, 23 terms through n = 23 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

static bool A071913_le(seq_t mid, long long j, seq_t k)
{
    /* true iff mid^j <= k, without overflow */
    seq_t p = 1;
    for (long long i = 0; i < j; i++) {
        if (p > k / mid) return false;
        p *= mid;
    }
    return true;
}

static seq_t A071913_root(seq_t k, long long j, int bits)
{
    seq_t lo = 1;
    seq_t hi = (seq_t)1 << (bits / (int)j + 1);
    while (lo < hi) {
        seq_t mid = lo + (hi - lo + 1) / 2;
        if (A071913_le(mid, j, k)) lo = mid;
        else hi = mid - 1;
    }
    return lo;
}

static long long A071913_count(seq_t k)
{
    int bits = 0;
    seq_t t = k;
    while (t > 0) { bits++; t >>= 1; }
    long long cnt = 0;
    seq_t prev = 0;
    for (long long j = 2; ; j++) {
        seq_t r = A071913_root(k, j, bits);
        if (r != prev) { cnt++; prev = r; }
        if (r == 1) break;
    }
    return cnt;
}

seq_t a(idx_t n)
{
    long long target = (long long)n;
    seq_t hi = 1;
    while (A071913_count(hi) < target) {
        seq_t nh = hi << 1;
        if (nh <= hi) return 0; /* does not fit */
        hi = nh;
    }
    seq_t lo = hi >> 1; /* count(lo) < target, or lo == 0 */
    /* smallest k in (lo, hi] with count >= target */
    while (hi - lo > 1) {
        seq_t mid = lo + (hi - lo) / 2;
        if (A071913_count(mid) >= target) hi = mid;
        else lo = mid;
    }
    return hi;
}
