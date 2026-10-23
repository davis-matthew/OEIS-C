/*
 * A035176  a(n) = Sum_{d|n} Kronecker(-14, d).
 * https://oeis.org/A035176
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) = Sum_{d|n} Kronecker(-14, d)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64, kronecker_neg14
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/factor_u64.h"
#include "../utils/kronecker_neg14.h"

static void add_divisor_sum(const uint64_t *pr, const int *ex,
                            size_t idx, size_t cnt, uint64_t cur,
                            seq_t *sum)
{
    if (idx == cnt) {
        int s = kronecker_neg14(cur);
        *sum += (seq_t)s;
        return;
    }
    uint64_t p = pr[idx];
    uint64_t d = cur;
    for (int e = 0; e <= ex[idx]; ++e) {
        add_divisor_sum(pr, ex, idx + 1, cnt, d, sum);
        d *= p;
    }
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;   /* undefined for n <= 0 */

    uint64_t primes[64];
    uint64_t exp[64];
    size_t cnt = 0;
    factor_u64((uint64_t)n, primes, exp, &cnt);

    int ex[64];
    for (size_t i = 0; i < cnt; ++i) {
        ex[i] = (int)exp[i];
    }

    seq_t sum = 0;
    add_divisor_sum(primes, ex, 0, cnt, 1ULL, &sum);
    return sum;
}
