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
 * Helpers:   int_root, pow_leq_uint64, pow_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <math.h>   /* for long double pow */
#include "../utils/int_root.h"
#include "../utils/pow_leq_uint64.h"
#include "../utils/pow_uint64.h"

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
        uint64_t pwr = pow_uint64(root, e);
        if (pwr > best && pwr <= N) best = pwr;
    }
    return (seq_t)(N - best);
}
