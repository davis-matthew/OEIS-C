/*
 * A220846  a(n) = sum_(d|n) ((product_(d|n) d) / d).
 * https://oeis.org/A220846
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum_(d|n) ((product_(d|n) d) / d)
 * Range:     u8 n <= 11, u16 n <= 23, u32 n <= 59, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Helpers:   big_pow, factor_u64, floor_isqrt_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include "../utils/factor_u64.h"   /* factor_u64 */
#include "../utils/big_pow.h"     /* big_pow */
#include "../utils/floor_isqrt_u64.h"  /* floor_isqrt_u64 */

/* Recursive enumeration of all divisors of n, accumulating a(n) */
static void dfs_divisors(size_t idx, uint64_t cur,
                         const uint64_t *pr, const uint64_t *ex,
                         size_t cnt, seq_t P, seq_t *sum)
{
    if (idx == cnt) {                     /* reached a complete divisor */
        *sum += P / (seq_t)cur;
        return;
    }
    uint64_t p = pr[idx];
    uint64_t div_val = 1;
    for (uint64_t e = 0; e <= ex[idx]; ++e) {
        dfs_divisors(idx + 1, cur * div_val,
                     pr, ex, cnt, P, sum);
        div_val *= p;
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;                 /* offset starts at 1 */

    uint64_t nn = (uint64_t)n;
    uint64_t pr[64];
    uint64_t ex[64];
    size_t cnt = 0;

    factor_u64(nn, pr, ex, &cnt);         /* factorization of n */

    /* divisor count τ(n) */
    idx_t tau = 1;
    for (size_t i = 0; i < cnt; ++i) {
        tau *= (ex[i] + 1);
    }

    /* product of divisors P = n^{τ/2} */
    seq_t P;
    if ((tau & 1) == 0) {                 /* τ even */
        P = big_pow((seq_t)nn, (seq_t)(tau >> 1));
    } else {                              /* τ odd – n is a perfect square */
        uint64_t sqrt_n = floor_isqrt_u64(nn);
        P = big_pow((seq_t)sqrt_n, (seq_t)tau);
    }

    seq_t sum = 0;
    dfs_divisors(0, 1, pr, ex, cnt, P, &sum);
    return sum;
}
