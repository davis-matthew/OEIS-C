/*
 * A054643  Primes prime(n) such that prime(n) + prime(n+1) + prime(n+2) == 0
 *          (mod 3).
 * https://oeis.org/A054643
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 938, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search for primes p such that p + next + next2 ≡ 0 (mod 3) */
/* a(n) is the first prime of the n‑th such triple, a(1)=3 */
#include <stdlib.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n == 0) {
        return 0; /* undefined, but safe */
    }

    /* prime array grows as needed */
    size_t primes_cap = 16;
    size_t primes_len = 0;
    uint64_t *primes = (uint64_t *)malloc(primes_cap * sizeof(uint64_t));
    if (!primes) return 0;

    /* seed the list with the first two primes */
    primes[primes_len++] = 2;
    primes[primes_len++] = 3;

    idx_t found = 0;          /* how many triples found so far */
    uint64_t candidate = 5;   /* next odd number to test */

    while (found < n) {
        /* test candidate for primality by trial division */
        bool is_prime = true;
        for (size_t i = 0; i < primes_len; ++i) {
            uint64_t p = primes[i];
            if (p > candidate / p)        /* p*p > candidate, stop */
                break;
            if (candidate % p == 0) {
                is_prime = false;
                break;
            }
        }

        if (is_prime) {
            /* ensure capacity */
            if (primes_len == primes_cap) {
                primes_cap *= 2;
                uint64_t *tmp = (uint64_t *)realloc(primes, primes_cap * sizeof(uint64_t));
                if (!tmp) {
                    free(primes);
                    return 0;
                }
                primes = tmp;
            }
            primes[primes_len++] = candidate;

            /* check the last triple only if we have at least 3 primes */
            if (primes_len >= 3) {
                uint64_t sum = primes[primes_len - 3] +
                               primes[primes_len - 2] +
                               primes[primes_len - 1];
                if (sum % 3 == 0) {
                    ++found;
                    if (found == n) {
                        seq_t result = (seq_t)primes[primes_len - 3];
                        free(primes);
                        return result;
                    }
                }
            }
        }
        candidate += 2; /* skip even numbers */
    }

    /* The loop always returns, but keep the compiler happy */
    free(primes);
    return 0;
}
