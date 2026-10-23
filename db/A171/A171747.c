/*
 * A171747  Smallest prime > n-th prime of the form 3*k-1.
 * https://oeis.org/A171747
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, less
 * Formula:   not stated
 * Range:     u8 n <= 27, u16 n <= 3282, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the smallest prime > the n-th prime of the form 3*k-1.
   The function builds a list of primes ≡ 2 (mod 3) using a sieve
   that grows until at least the requested number are found, then
   returns the next prime after the nth such prime. */
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include "../utils/is_prime_small.h"

seq_t a(idx_t n)
{
    /* static state for caching the primes of the form 3*k-1 */
    static idx_t sieve_limit = 0;        /* current maximum sieved value   */
    static seq_t *mod_primes = NULL;     /* array of qualifying primes     */
    static size_t mod_count = 0;         /* number of primes stored        */

    if (n == 0)
        return 0;   /* undefined for n==0, harmless */

    /* Grow the sieve until we have at least n such primes. */
    if (mod_count < (size_t)n) {
        /* initial lower bound if not set yet */
        if (sieve_limit == 0)
            sieve_limit = 200000;  /* sufficient for n ≈ 10 000 */

        /* Increase size until we accumulate enough primes. */
        while (mod_count < (size_t)n) {
            sieve_limit *= 2;                    /* double the limit */

            /* Allocate composite flag array. */
            uint8_t *is_comp = (uint8_t*)malloc((size_t)(sieve_limit + 1));
            if (!is_comp)
                return 0;                            /* out of memory */
            memset(is_comp, 0, (size_t)(sieve_limit + 1));

            /* Free previously stored list. */
            free(mod_primes);
            mod_primes = NULL;
            mod_count = 0;

            /* Rough estimate for needed capacity. */
            size_t cap = (size_t)(sieve_limit / 10) + 10;
            mod_primes = (seq_t*)malloc(cap * sizeof(seq_t));
            if (!mod_primes) {
                free(is_comp);
                return 0;
            }

            size_t idx = 0;
            for (idx_t i = 2; i <= sieve_limit; ++i) {
                if (!is_comp[(size_t)i]) {
                    /* i is a prime */
                    if ((seq_t)i % 3 == 2) {
                        if (idx == cap) {
                            cap <<= 1;                      /* grow */
                            seq_t *tmp = (seq_t*)realloc(mod_primes, cap * sizeof(seq_t));
                            if (!tmp) {
                                free(is_comp);
                                free(mod_primes);
                                return 0;
                            }
                            mod_primes = tmp;
                        }
                        mod_primes[idx++] = (seq_t)i;
                    }
                    if (i > sieve_limit / i)
                        continue;
                    for (idx_t j = i * i; j <= sieve_limit; j += i)
                        is_comp[(size_t)j] = 1;
                }
            }

            mod_count = idx;
            free(is_comp);

            /* resize array to exact size */
            seq_t *tmp = (seq_t*)realloc(mod_primes, mod_count * sizeof(seq_t));
            if (tmp || mod_count == 0)
                mod_primes = tmp;
        }
    }

    /* nth qualifying prime (0‑based index n-1). */
    seq_t target = mod_primes[(size_t)(n - 1)];
    seq_t candidate = target + 1;

    /* Find the next prime after the target. */
    while (!is_prime_small(candidate))
        ++candidate;

    return candidate;
}
