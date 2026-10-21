/*
 * A066461  a(3) = 5; a(n) = min(x>1,x^2+x*n+n^2 = 1 mod(x+n)).
 * https://oeis.org/A066461
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = y - n * where y is the least divisor of n^2‑1 greater
 *            than n+1. * * Algorithm: * 1. factor n-1 and n+1 into primes; *
 *            2. generate all divisors of (n-1) and of (n+1); * 3. enumerate
 *            all products d1*d2 (d1 divides n-1, d2 divides n+1); * keep the
 *            smallest product that exceeds n+1; * 4. retur...
 * Range:     u8 n <= 17, u16 n <= 269, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   gen_divisors_rec
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/gen_divisors_rec.h"

static void factor_uint64(uint64_t n, uint64_t *pr, uint32_t *exp,
                          size_t *cnt)
{
    size_t i = 0;
    for (uint64_t p = 2; p * p <= n; ++p) {
        if (n % p == 0) {
            uint32_t e = 0;
            while (n % p == 0) {
                n /= p;
                ++e;
            }
            pr[i]   = p;
            exp[i]  = e;
            ++i;
        }
    }
    if (n > 1) {
        pr[i]   = n;
        exp[i]  = 1;
        ++i;
    }
    *cnt = i;
}

static void gen_divisors(const uint64_t *pr, const uint32_t *exp,
                         size_t cnt, uint64_t *out, size_t *c)
{
    gen_divisors_rec(pr, exp, cnt, 0, 1, out, c);
}

seq_t a(idx_t n)
{
    if (n < 3)            /* behaviour below the offset is irrelevant */
        return (seq_t)0;

    uint64_t nn = (uint64_t)n;

    /* factor n-1 and n+1 */
    uint64_t pr1[64], pr2[64];
    uint32_t exp1[64], exp2[64];
    size_t cnt1, cnt2;
    factor_uint64(nn - 1, pr1, exp1, &cnt1);
    factor_uint64(nn + 1, pr2, exp2, &cnt2);

    /* divisors of (n-1) and (n+1) */
    uint64_t div1[1024], div2[1024];
    size_t c1 = 0, c2 = 0;
    gen_divisors(pr1, exp1, cnt1, div1, &c1);
    gen_divisors(pr2, exp2, cnt2, div2, &c2);

    __uint128_t best = (__uint128_t)-1;          /* upper bound (max value) */
    const __uint128_t lim = ( (__uint128_t)nn + 1 );

    for (size_t i = 0; i < c1; ++i) {
        for (size_t j = 0; j < c2; ++j) {
            __uint128_t prod = (__uint128_t)div1[i] * (__uint128_t)div2[j];
            if (prod <= lim)
                continue;
            if (prod < best)
                best = prod;
        }
    }

    /* best is guaranteed to be found (at least n^2-1 itself) */
    uint64_t ans = (uint64_t)(best - nn);
    return (seq_t)ans;
}
