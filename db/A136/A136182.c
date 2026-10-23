/*
 * A136182  a(n) = Product_{k=1..d(n)-1} lcm(b(k), b(k+1)), where b(k) is the
 *          k-th positive divisor of n and d(n) = the number of positive
 *          divisors of n.
 * https://oeis.org/A136182
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ∏_{k=1}^{d(n)-1} lcm(b_k , b_{k+1}) where b_k are the
 *            positive divisors of n arranged increasingly
 * Range:     u8 n <= 11, u16 n <= 23, u32 n <= 47, u64 n <= 119,
 *            u128 n <= 359, big any n
 * Helpers:   gcd_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/gcd_seq.h"

/* Integer square root of an unsigned 64–bit integer. */
static inline idx_t isqrt_u64(idx_t n)
{
    idx_t lo = 0, hi = n, ans = 0;
    while (lo <= hi) {
        idx_t mid = lo + ((hi - lo) >> 1);
        if (mid == 0) { ans = 0; lo = 1; continue; }
        if (mid > n / mid) hi = mid - 1;
        else { ans = mid; lo = mid + 1; }
    }
    return ans;
}

/* Euclidean algorithm for seq_t (works for built‑in unsigned types
   and arbitrary‑precision integer types defined as seq_t). */

/* Least common multiple of two seq_t values. */
static inline seq_t lcm_seq(seq_t a, seq_t b)
{
    seq_t g = gcd_seq(a, b);
    return (a / g) * b;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)1;

    idx_t sq = isqrt_u64(n);
    seq_t small[512];
    seq_t large[512];
    int small_cnt = 0, large_cnt = 0;

    for (idx_t i = 1; i <= sq; ++i) {
        if (n % i == 0) {
            small[small_cnt++] = (seq_t)i;
            idx_t j = n / i;
            if (j != i) large[large_cnt++] = (seq_t)j;
        }
    }

    seq_t prod = (seq_t)1;
    seq_t prev = small[0];

    for (int i = 1; i < small_cnt; ++i) {
        seq_t curr = small[i];
        prod = prod * lcm_seq(prev, curr);
        prev = curr;
    }

    for (int i = large_cnt - 1; i >= 0; --i) {
        seq_t curr = large[i];
        prod = prod * lcm_seq(prev, curr);
        prev = curr;
    }

    return prod;
}
