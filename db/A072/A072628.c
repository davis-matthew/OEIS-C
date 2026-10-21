/*
 * A072628  Number of divisors d of n such that d-1 is not prime.
 * https://oeis.org/A072628
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of divisors d of n such that d-1 is not prime
 * Range:     u8 n <= 747, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include "../utils/factor_u64.h"
#include "../utils/is_prime.h"

static void rec_divisors(size_t idx, size_t cnt,
                         const uint64_t *primes,
                         const uint64_t *exps,
                         seq_t current, seq_t *cnt_nonprime)
{
    if (idx == cnt) {
        seq_t rem = current - 1;          /* current ≥ 1 */
        if (!is_prime((idx_t)rem))
            (*cnt_nonprime)++;
        return;
    }

    seq_t pow = 1;
    for (size_t e = 0; e <= exps[idx]; ++e) {
        rec_divisors(idx + 1, cnt, primes, exps,
                     current * pow, cnt_nonprime);
        pow *= primes[idx];
    }
}

seq_t a(idx_t n)
{
    uint64_t primes[64], exps[64];
    size_t nf = 0;

    /* factorize n */
    factor_u64((uint64_t)n, primes, exps, &nf);

    seq_t result = 0;
    rec_divisors(0, nf, primes, exps,
                 (seq_t)1, &result);
    return result;
}
