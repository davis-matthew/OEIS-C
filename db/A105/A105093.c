/*
 * A105093  Numbers n such that n = prime(k) + prime(k+3) = prime(k+1) +
 *          prime(k+2) for some k.
 * https://oeis.org/A105093
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the nth number that can be written as the sum of two
 *            consecutive pairs of primes with equal gaps: prime(k) +
 *            prime(k+3) = prime(k+1) + prime(k+2) for some integer k. The
 *            sequence starts: 18, 24, 30, 36, 60, …
 * Range:     u8 n <= 12, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* Local primes list – rebuilt on every call */
    size_t primes_cap = 64;
    size_t primes_cnt = 0;
    uint64_t *primes = (uint64_t *)malloc(primes_cap * sizeof(uint64_t));
    if (!primes) return (seq_t)0;  /* out of memory – never expected */

    uint64_t next_candidate = 2;
    seq_t result = 0;
    size_t terms_found = 0;

    while (terms_found < n) {
        /* Legally test if next_candidate is prime */
        int is_prime = 1;
        for (size_t i = 0; i < primes_cnt; ++i) {
            uint64_t p = primes[i];
            if ((unsigned __int128)p * p > next_candidate)
                break;
            if (next_candidate % p == 0) {
                is_prime = 0;
                break;
            }
        }

        if (is_prime) {
            /* Grow array if needed */
            if (primes_cnt == primes_cap) {
                primes_cap *= 2;
                uint64_t *tmp = (uint64_t *)realloc(primes, primes_cap * sizeof(uint64_t));
                if (!tmp) { free(primes); return (seq_t)0; }
                primes = tmp;
            }
            primes[primes_cnt++] = next_candidate;

            /* Check the gap condition once we have at least four primes */
            if (primes_cnt >= 4) {
                size_t k = primes_cnt - 4;
                uint64_t g1 = primes[k + 1] - primes[k];
                uint64_t g2 = primes[k + 3] - primes[k + 2];
                if (g1 == g2) {
                    result = (seq_t)primes[k] + (seq_t)primes[k + 3];
                    ++terms_found;
                    if (terms_found == n) break;
                }
            }
        }

        /* Advance to the next candidate number */
        if (next_candidate == 2)
            next_candidate = 3;
        else
            next_candidate += 2;
    }

    free(primes);
    return result;
}
