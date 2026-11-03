/*
 * A217895  Sum of d/Gpf(d) for all divisors d of n, with Gpf(d) the greatest
 *          prime factor of d.
 * https://oeis.org/A217895
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum_{d|n} d / Gpf(d), with Gpf(1)=1
 * Range:     u8 n <= 255, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 93 terms through n = 9709 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

/* recursively enumerate all divisors of n, accumulating the sum of
   divisor / greatest prime factor of the divisor.  cur is the current
   divisor, cur_max is the largest prime factor encountered so far. */
static seq_t
_sum_div_gpf(const uint64_t *pf, const uint64_t *ef, size_t cnt,
             size_t idx, uint64_t cur, uint64_t cur_max)
{
    if (idx == cnt)                      /* all prime factors processed */
        return (seq_t)(cur / cur_max);

    seq_t s = 0;

    /* exponent 0 for this prime factor */
    s += _sum_div_gpf(pf, ef, cnt, idx + 1, cur, cur_max);

    /* positive exponents */
    uint64_t prod = cur;
    for (uint64_t e = 1; e <= ef[idx]; ++e) {
        prod *= pf[idx];                 /* multiply by next power of the prime  */
        uint64_t newmax = pf[idx] > cur_max ? pf[idx] : cur_max;
        s += _sum_div_gpf(pf, ef, cnt, idx + 1, prod, newmax);
    }
    return s;
}

/* a(n) = sum of d / Gpf(d) over all divisors d of n  (1‑based indexing) */
seq_t a(idx_t n)
{
    if (n < 1)                /* offset is 1; values below are irrelevant */
        return 0;

    uint64_t nn = (uint64_t)n;

    /* factor n into prime powers */
    uint64_t pf[64];          /* primes dividing n */
    uint64_t ef[64];          /* corresponding exponents */
    size_t cnt = 0;
    uint64_t tmp = nn;

    for (uint64_t p = 2; p <= tmp / p; ++p) {   /* trial division up to sqrt */
        if (tmp % p == 0) {
            pf[cnt] = p;
            uint64_t e = 0;
            while (tmp % p == 0) {
                tmp /= p;
                ++e;
            }
            ef[cnt] = e;
            ++cnt;
        }
    }
    if (tmp > 1) {                  /* remaining prime factor > sqrt */
        pf[cnt] = tmp;
        ef[cnt] = 1;
        ++cnt;
    }

    /* compute the desired sum */
    return _sum_div_gpf(pf, ef, cnt, 0, 1, 1);
}
