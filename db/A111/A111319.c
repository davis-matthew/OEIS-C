/*
 * A111319  Numbers k such that 5 divides prime(1) + ... + prime(k).
 * https://oeis.org/A111319
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the index k of the k‑th prime such that the sum of the
 *            first k primes is congruent to 0 modulo 5 (OEIS A111319). The
 *            computation generates primes one by one, accumulates the running
 *            sum modulo 5, and counts how many times the remainder becomes 0.
 *            When the counter reaches n, the current pr...
 * Range:     u8 n <= 40, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   integer_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/integer_sqrt.h"

/* Return the n-th term of sequence A111319 (n≥1). */
seq_t a(idx_t n)
{
    if (n < 1)
        return 0;

    /* Prime sieve of Eratosthenes across odd numbers.
       We need only the list of primes to test primality of candidates. */
    size_t prime_cap = 16;
    size_t prime_sz  = 0;
    uint64_t *primes = (uint64_t *)malloc(prime_cap * sizeof(uint64_t));
    if (!primes) /* out of memory; return 0 to keep compile‑time safety */
        return 0;

    uint64_t sum_mod = 0;   /* running sum modulo 5 */
    idx_t    k      = 0;    /* current prime index (starts at 0, will be 1 for prime 2) */
    idx_t    count  = 0;    /* how many times sum_mod has been 0 */

    uint64_t cand = 2;      /* candidate to test for primality */

    for (;;)
    {
        /* Test cand for primality using the primes found so far.
           Only trial division by primes <= sqrt(cand). */
        bool is_prime = true;
        if (cand > 2) {
            idx_t lim = integer_sqrt(cand);
            for (size_t i = 0; i < prime_sz && primes[i] <= lim; ++i) {
                if (cand % primes[i] == 0) {
                    is_prime = false;
                    break;
                }
            }
        }

        if (is_prime) {
            /* Store the new prime for future tests. */
            if (prime_sz == prime_cap) {
                prime_cap <<= 1;
                uint64_t *tmp = (uint64_t *)realloc(primes, prime_cap * sizeof(uint64_t));
                if (!tmp) { free(primes); return 0; }
                primes = tmp;
            }
            primes[prime_sz++] = cand;

            /* Update prime index and running sum modulo 5. */
            ++k;
            sum_mod += cand % 5;
            sum_mod %= 5;
            if (sum_mod == 0) {
                ++count;
                if (count == n) {
                    free(primes);
                    return (seq_t)k;
                }
            }
        }

        /* Advance to the next candidate: 2, then only odd numbers. */
        cand = (cand == 2) ? 3 : cand + 2;
    }
}
