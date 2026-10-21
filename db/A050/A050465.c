/*
 * A050465  a(n) = Sum_{d|n, n/d=3 mod 4} d^2.
 * https://oeis.org/A050465
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{d|n, n/d ≡ 3 (mod 4)} d^2
 * Range:     u8 n <= 47, u16 n <= 692, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This uses factors of n and enumerates all divisors recursively. */
#include <stdint.h>
#include "../utils/factor_u64.h"

/* Recursive helper: walk through the prime factorisation and enumerate
   all divisors.  When a divisor is finished, if its complementary divisor
   (n / d) ≡ 3 (mod 4) then add d² to the running sum. */
static void dfs_divs(uint64_t n,
                     const uint64_t *pr, const uint64_t *exp,
                     size_t idx, size_t cnt,
                     uint64_t cur, seq_t *sum)
{
    if (idx == cnt) {
        uint64_t k = n / cur;            /* complementary divisor */
        if ((k & 3ULL) == 3ULL) {          /* k % 4 == 3 */
            seq_t d = (seq_t)cur;
            *sum += d * d;               /* d² within seq_t arithmetic */
        }
        return;
    }

    uint64_t p = pr[idx];
    uint64_t e = exp[idx];
    uint64_t prod = 1ULL;                /* pⁱ */
    for (uint64_t i = 0; i <= e; ++i) {
        dfs_divs(n, pr, exp, idx + 1, cnt, cur * prod, sum);
        prod *= p;
    }
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;

    /* factorise n */
    uint64_t pr[64];
    uint64_t exp[64];
    size_t cnt = 0;
    factor_u64((uint64_t)n, pr, exp, &cnt);

    seq_t total = 0;
    dfs_divs((uint64_t)n, pr, exp, 0, cnt, 1ULL, &total);
    return total;
}
