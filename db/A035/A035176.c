/*
 * A035176  a(n) = Sum_{d|n} Kronecker(-14, d).
 * https://oeis.org/A035176
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) = Sum_{d|n} Kronecker(-14, d)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 121 terms through n = 8358 (2026-11-01)
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/factor_u64.h"

static inline int kronecker_neg14(uint64_t d)
{
    /* If d is even or divisible by 7 the symbol is 0. */
    if ((d & 1ULL) == 0ULL) return 0;
    if (d % 7ULL == 0ULL)  return 0;

    /* (-1|d) = (-1)^{(d-1)/2} */
    int val = ((d & 3ULL) == 1ULL) ? 1 : -1;   /* d%4==1 -> 1, d%4==3 -> -1 */

    /* (2|d) for odd d */
    switch (d % 8ULL) {
        case 1ULL: case 7ULL: break;           /* unchanged */
        case 3ULL: case 5ULL: val = -val;      /* flip sign */
        default: break;                        /* d is odd, only 1,3,5,7 */
    }

    /* (7|d) via quadratic reciprocity:
       (7|d) = (d|7) * (-1)^{(d-1)/2}   because 7 ≡ 3 (mod 4).   */
    int mod7 = d % 7ULL;
    int leg_d7 = (mod7 == 1 || mod7 == 2 || mod7 == 4) ? 1 : -1;
    if ((d & 3ULL) == 3ULL) leg_d7 = -leg_d7;   /* multiply by (-1)^{(d-1)/2} */
    val *= leg_d7;

    return val;   /* -1, 0, or +1 */
}

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
