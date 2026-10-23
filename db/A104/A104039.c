/*
 * A104039  Number of primitive roots modulo prime(n)^2, where prime(n) is
 *          n-th prime.
 * https://oeis.org/A104039
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (p_n - 1) * φ(p_n - 1) where p_n is the n‑th prime
 * Range:     u8 n <= 9, u16 n <= 75, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   phi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/phi.h"

seq_t a(idx_t n)
{
    if (n == 0) {                    /* offset is 1; undefined for 0 */
        return 0;
    }

    /* static state for incremental prime generation */
    static idx_t generated = 0;          /* number of primes generated so far */
    static uint64_t *primes = NULL;      /* array of primes */
    static size_t cap = 0;               /* capacity of the array */
    static uint64_t candidate = 2;       /* next candidate to test */

    /* generate primes until we have the n‑th prime */
    while (generated < n) {
        /* test candidate for primality */
        bool is_prime = true;

        if (candidate == 2) {
            /* 2 is prime */
        } else {
            /* trial division by known primes up to sqrt(candidate) */
            for (size_t i = 0; i < generated && (uint64_t)primes[i] * primes[i] <= candidate; ++i) {
                if (candidate % primes[i] == 0) {
                    is_prime = false;
                    break;
                }
            }
        }

        if (is_prime) {
            /* store the new prime */
            if (generated == cap) {
                /* grow capacity – double, start at 4 */
                cap = cap ? cap * 2 : 4;
                uint64_t *new_arr = (uint64_t *)realloc(primes, cap * sizeof(uint64_t));
                if (!new_arr) {            /* allocation failed */
                    free(primes);
                    primes = NULL;
                    return 0;
                }
                primes = new_arr;
            }
            primes[generated] = candidate;
            ++generated;
        }

        /* move to the next candidate: 2, then only odd numbers */
        candidate = (candidate == 2) ? 3 : candidate + 2;
    }

    /* now primes[n-1] is the n-th prime */
    uint64_t p = primes[n - 1];
    uint64_t m = p - 1;                 /* m = p - 1 */

    seq_t phi_m = phi(m);               /* φ(m) */
    return (seq_t)m * phi_m;
}
