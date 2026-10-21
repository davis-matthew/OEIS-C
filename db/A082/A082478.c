/*
 * A082478  Number of divisors d of n such that (d+1) is a divisor of (n+1)
 *          and (d+2) a divisor of (n+2).
 * https://oeis.org/A082478
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = #{ d | n : (d+1)|(n+1) and (d+2)|(n+2) }
 * Range:     u8 n <= 9548, u16 n <= 9548, u32 n <= 9548, u64 n <= 9548,
 *            u128 n <= 9548, big any n
 * Helpers:   factor
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Uses the provided factorisation routine from ../utils/factor.h */

#include <stdint.h>
#include <stddef.h>
#include "../utils/factor.h"

static void count_divisors(size_t idx, uint64_t cur,
                           const uint64_t *primes, const uint64_t *exps,
                           size_t n_primes,
                           uint64_t n1, uint64_t n2,
                           uint64_t *count)
{
    if (idx == n_primes) {
        /* cur is a divisor of n */
        if ((n1 % (cur + 1)) == 0 && (n2 % (cur + 2)) == 0)
            ++(*count);
        return;
    }
    uint64_t p = primes[idx];
    uint64_t exp = exps[idx];
    uint64_t mul = 1;
    for (uint64_t e = 0; e <= exp; ++e) {
        count_divisors(idx + 1, cur * mul,
                       primes, exps, n_primes,
                       n1, n2, count);
        mul *= p;
    }
}

seq_t a(idx_t n)
{
    uint64_t nn = (uint64_t)n;          /* n ≥ 1 */
    uint64_t primes[64], exps[64];
    size_t n_primes;
    factor(nn, primes, exps, &n_primes);

    uint64_t n1 = nn + 1;
    uint64_t n2 = nn + 2;
    uint64_t cnt = 0;
    count_divisors(0, 1, primes, exps, n_primes, n1, n2, &cnt);
    return (seq_t)cnt;
}
