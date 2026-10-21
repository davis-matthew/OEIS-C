/*
 * A072298  Smallest integer k such that for no integer m, m! is divisible by
 *          n^k but not by n^(k+1).
 * https://oeis.org/A072298
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = smallest k such that for no integer m, m! is divisible by
 *            n^k but not by n^(k+1). The function iterates over m, computes
 *            the exponent of each prime factor of n in m!, and finds the
 *            first unachievable exponent
 * Range:     u8 n <= 242, u16 n <= 2000, u32 n <= 2000, u64 n <= 2000,
 *            u128 n <= 2000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

/* helper: factor n into primes[] and exps[] (max 16 distinct primes). */
static void factor_uint64(uint64_t n, uint64_t *pr, uint64_t *exps, size_t *cnt)
{
    size_t i = 0;
    for (uint64_t p = 2; p * p <= n; ++p) {
        if (n % p == 0) {
            uint64_t e = 0;
            while (n % p == 0) {
                n /= p;
                ++e;
            }
            pr[i] = p;
            exps[i] = e;
            ++i;
        }
    }
    if (n > 1) {
        pr[i] = n;
        exps[i] = 1;
        ++i;
    }
    *cnt = i;
}

seq_t a(idx_t n)
{
    if (n < 2) return (seq_t)0;          /* offset starts at 2 */

    /* factor n */
    uint64_t primes[16], exps[16];
    size_t pcnt;
    factor_uint64((uint64_t)n, primes, exps, &pcnt);

    uint64_t counts[16] = {0};      /* exponents of each prime in m! */
    uint64_t prev = 0;              /* λ(m-1) for current m */

    /* iterate over m starting from 2 */
    for (idx_t m = 2; ; ++m) {
        /* add exponent of each prime factor of n that appears in m */
        for (size_t i = 0; i < pcnt; ++i) {
            uint64_t p = primes[i];
            uint64_t temp = m;
            uint64_t add = 0;
            while (temp % p == 0) {
                ++add;
                temp /= p;
            }
            counts[i] += add;
        }

        /* compute λ(m) = min_i floor(counts[i] / exps[i]) */
        uint64_t lam = UINT64_MAX;
        for (size_t i = 0; i < pcnt; ++i) {
            uint64_t val = counts[i] / exps[i];
            if (val < lam) lam = val;
        }

        if (lam > prev + 1)          /* first missing exponent found */
            return (seq_t)(prev + 1);
        if (lam > prev)              /* λ grew by 1 */
            prev = lam;
        /* if lam == prev : nothing changes, continue */
    }
}
