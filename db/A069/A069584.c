/*
 * A069584  a(n) = n - largest perfect power <= n.
 * https://oeis.org/A069584
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = n - largest perfect power <= n (perfect powers are a^k
 *            with k>=2)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 111 terms through n = 8826 (2026-11-01)
 * Helpers:   ipow_u64_i32_to_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <math.h>   /* for long double pow */
#include "../utils/ipow_u64_i32_to_u64.h"

static inline bool pow_leq_uint64(uint64_t base, int exp, uint64_t limit)
{
    __uint128_t prod = 1;
    for (int i = 0; i < exp; ++i) {
        prod *= base;
        if (prod > limit) return false;
    }
    return true;
}

/* integer e‑th root: floor(n^(1/e)) */
static inline uint64_t int_root(uint64_t n, int e)
{
    if (e == 1) return n;
    long double approx = pow((long double)n, 1.0L / e);
    uint64_t high = (uint64_t)approx + 2;
    if (high > n) high = n;
    uint64_t low = 1;
    while (low < high) {
        uint64_t mid = (low + high + 1) >> 1;
        if (pow_leq_uint64(mid, e, n)) low = mid;
        else high = mid - 1;
    }
    return low;
}

seq_t a(idx_t n)
{
    uint64_t N = (uint64_t)n;      /* idx_t is uint64_t */
    if (N <= 1) return (seq_t)0;   /* 1 is a perfect power (1^k) */

    uint64_t best = 1;             /* smallest perfect power */
    /* maximal exponent is floor(log2 N) */
    int max_exp = 0;
    uint64_t tmp = N;
    while (tmp > 1) { tmp >>= 1; ++max_exp; }
    if (max_exp < 2) max_exp = 2;

    for (int e = max_exp; e >= 2; --e) {
        uint64_t root = int_root(N, e);
        if (root <= 1) continue;  /* root 1 gives power 1 already handled */
        uint64_t pwr = ipow_u64_i32_to_u64(root, e);
        if (pwr > best && pwr <= N) best = pwr;
    }
    return (seq_t)(N - best);
}
