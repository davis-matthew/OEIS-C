/*
 * A106949  Primes of the form 2x^2 + 9y^2.
 * https://oeis.org/A106949
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 15, u16 n <= 1643, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime_64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Sequence A106949: Primes that can be expressed as 2x² + 9y².
 * For primes > 3 this is equivalent to primes congruent to 11 or 17 modulo 24.
 * The prime 2 also satisfies the form, so it is included.
 *
 * The implementation builds the list lazily, using the helper
 * `is_prime_64` from <../utils/is_prime_64.h>.
 */
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/is_prime_64.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;   /* undefined behaviour for n == 0; keep return */

    /* static cache of found primes */
    static seq_t *primes = NULL;
    static size_t  count  = 0;
    static size_t  cap    = 0;

    /* Allocate or grow the cache */
    if (count >= cap) {
        size_t new_cap = cap ? (cap << 1) : 32;
        seq_t *tmp = (seq_t *)realloc(primes, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        primes = tmp;
        cap = new_cap;
    }

    /* Return cached value if already computed */
    if (count >= n)
        return primes[n - 1];

    /* Start candidate search */
    uint64_t candidate;
    if (count == 0)
        candidate = 2;
    else
        candidate = (uint64_t)primes[count - 1] + 1;

    while (count < n) {
        if (candidate <= 2) {
            /* The prime 2 satisfies the residue 2 modulo 24. */
            if (candidate == 2) {
                primes[count++] = (seq_t)candidate;
            }
            candidate = 3;
            continue;
        }

        /* Skip even numbers */
        if ((candidate & 1ULL) == 0) {
            candidate++;
            continue;
        }

        /* Check residue modulo 24 */
        uint64_t mod24 = candidate % 24ULL;
        if (mod24 == 11ULL || mod24 == 17ULL) {
            if (is_prime_64(candidate)) {
                /* Store prime */
                primes[count++] = (seq_t)candidate;
                if (count >= cap) {
                    size_t new_cap = cap << 1;
                    seq_t *tmp = (seq_t *)realloc(primes,
                                                  new_cap * sizeof(seq_t));
                    if (!tmp) abort();
                    primes = tmp;
                    cap = new_cap;
                }
            }
        }
        candidate += 2;  /* jump to next odd number */
    }

    return primes[n - 1];
}
