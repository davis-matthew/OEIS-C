/*
 * A173439  Number of divisors d of number n such that sigma(d) divides
 *          sigma(n).
 * https://oeis.org/A173439
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 388, u16 n <= 8077, u32 n <= 8077, u64 n <= 8077,
 *            u128 n <= 8077, big any n
 * Helpers:   factor_u64, sigma_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation of OEIS A173439: number of divisors d of n such that sigma(d) divides sigma(n) */
#include <stddef.h>
#include <stdint.h>
#include "../utils/sigma_of.h"
#include "../utils/factor_u64.h"

static void enumerate_divisors(size_t idx, seq_t divisor,
                               const uint64_t *primes, const uint64_t *exps,
                               size_t cnt, seq_t sigma_n, seq_t *out)
{
    if (idx == cnt) {
        seq_t sigma_d = sigma_of((idx_t)divisor);
        if (sigma_n % sigma_d == 0)
            ++(*out);
        return;
    }

    seq_t power = 1;
    for (uint64_t e = 0; e <= exps[idx]; ++e) {
        enumerate_divisors(idx + 1, divisor * power, primes, exps, cnt, sigma_n, out);
        power = power * primes[idx];
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;                 /* outside the OEIS offset */

    seq_t sigma_n = sigma_of(n);

    uint64_t primes[16], exps[16];
    size_t num = 0;
    factor_u64((uint64_t)n, primes, exps, &num);

    seq_t count = 0;
    enumerate_divisors(0, (seq_t)1, primes, exps, num, sigma_n, &count);
    return count;
}
