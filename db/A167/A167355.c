/*
 * A167355  Totally multiplicative sequence with a(p) = (p-2)*(p+2) = p^2-4
 *          for prime p.
 * https://oeis.org/A167355
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = product_{p^e||n} (p^2-4)^e (a multiplicative function)
 *            a(1) = 1, a(2k) = 0 for k ≥ 1
 * Range:     u8 n <= 16, u16 n <= 256, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    if (n == 1u) return (seq_t)1;

    /* If n is even the factor 2 gives (2^2-4)=0, so the whole product is 0. */
    if ((n & 1u) == 0u) return (seq_t)0;

    /* Factor n by simple trial division (fast enough for n up to ~10⁵). */
    uint64_t tmp = (uint64_t)n;
    uint64_t primes[16];
    uint64_t exps[16];
    size_t cnt = 0;

    /* skip factor 2 (already handled) */
    for (uint64_t d = 3; d * d <= tmp; d += 2) {
        if (tmp % d == 0) {
            uint64_t e = 0;
            while (tmp % d == 0) {
                tmp /= d;
                e++;
            }
            primes[cnt] = d;
            exps[cnt] = e;
            cnt++;
        }
    }
    if (tmp > 1) {            /* remaining prime factor */
        primes[cnt] = tmp;
        exps[cnt] = 1;
        cnt++;
    }

    /* Compute the multiplicative product. */
    seq_t result = 1;
    for (size_t i = 0; i < cnt; ++i) {
        seq_t base = (seq_t)(primes[i] * primes[i] - 4); /* (p^2-4) */
        seq_t pow  = 1;
        for (uint64_t e = 0; e < exps[i]; ++e) pow *= base;
        result *= pow;
    }

    return result;
}
