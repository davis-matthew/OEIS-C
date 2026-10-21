/*
 * A064028  Sum of the unitary divisors of n!.
 * https://oeis.org/A064028
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ∏_{p ≤ n} (1 + p^{∑_{k≥1} ⌊n/p^k⌋})
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 12, u64 n <= 20, u128 n <= 33,
 *            big any n
 * Helpers:   build_primes
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include "../utils/build_primes.h"

static idx_t *prime_list = NULL;
static size_t prime_count = 0;
static idx_t prime_limit = 0;

static void ensure_primes(idx_t limit)
{
    if (limit <= prime_limit) return;
    if (prime_list) free(prime_list);
    build_primes(limit, &prime_list, &prime_count);
    prime_limit = limit;
}

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;
    ensure_primes(n);

    seq_t result = (seq_t)1;
    for (size_t i = 0; i < prime_count; ++i)
    {
        idx_t p = prime_list[i];

        /* exponent of p in n! */
        idx_t e = 0;
        idx_t power = p;
        while (power <= n)
        {
            e += n / power;
            if (power > n / p) break;
            power *= p;
        }

        seq_t p_pow = (seq_t)1;
        for (idx_t j = 0; j < e; ++j)
            p_pow *= (seq_t)p;

        result *= ((seq_t)1 + p_pow);
    }
    return result;
}
