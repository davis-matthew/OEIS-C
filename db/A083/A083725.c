/*
 * A083725  a(n) = n * [1 + sum(k=1 to n) prime(k)].
 * https://oeis.org/A083725
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n * (1 + sum_{k=1..n} prime(k))
 * Range:     u8 n <= 6, u16 n <= 32, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   ensure_capacity
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/ensure_capacity.h"

seq_t a(idx_t n)
{
    if (n == 0u) return (seq_t)0;

    /* Static storage for primes and their cumulative sums. */
    static seq_t *primes = NULL;
    static size_t primes_cnt = 0, primes_cap = 0;
    static seq_t *cumSum = NULL;
    static size_t cum_cap = 0;
    static uint64_t next_candidate = 2;

    size_t need = (size_t)n;
    if (primes_cnt < need)
    {
        while (primes_cnt < need)
        {
            uint64_t x = next_candidate;
            bool is_prime = true;

            /* Trial division by known primes up to sqrt(x). */
            for (size_t j = 0; j < primes_cnt &&
                 (uint64_t)primes[j] * (uint64_t)primes[j] <= x; ++j)
            {
                if (x % (uint64_t)primes[j] == 0)
                {
                    is_prime = false;
                    break;
                }
            }

            if (is_prime)
            {
                ensure_capacity(primes_cnt + 1, &primes_cap,
                                (void **)&primes, sizeof(seq_t));
                ensure_capacity(primes_cnt + 1, &cum_cap,
                                (void **)&cumSum, sizeof(seq_t));

                primes[primes_cnt] = (seq_t)x;
                if (primes_cnt == 0)
                    cumSum[0] = (seq_t)x;
                else
                    cumSum[primes_cnt] = cumSum[primes_cnt - 1] + (seq_t)x;

                ++primes_cnt;
            }
            ++next_candidate;
        }
    }

    seq_t sn = (seq_t)n;
    seq_t sumPrimes = cumSum[need - 1];
    return sn * ((seq_t)1 + sumPrimes);
}
