/*
 * A045367  Primes congruent to {1, 2, 5} mod 7.
 * https://oeis.org/A045367
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 26, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Method: incremental search for primes ≡ 1, 2, 5 (mod 7) */
#include <stdlib.h>
#include "../utils/is_prime_big.h"

/* Cached primes of the desired residue classes */
static seq_t *primes = NULL;   /* array of primes */
static size_t primes_len = 0;  /* number of primes generated */
static size_t primes_cap = 0;  /* allocated capacity of the array */

/* Return the n-th prime that is ≡ 1, 2, or 5 (mod 7). */
seq_t a(idx_t n)
{
    /* OEIS offset is 1; n == 0 is undefined but we return 0 */
    if (n == 0) return 0;

    /* Generate more primes if necessary. */
    while (primes_len < (size_t)n) {
        seq_t cand;
        if (primes_len == 0)
            cand = 2;                     /* first prime candidate */
        else
            cand = primes[primes_len - 1] + 1;

        /* Search for the next qualifying prime. */
        for (;;) {
            int rem = (int)(cand % 7);
            if (rem != 1 && rem != 2 && rem != 5) {
                ++cand;                    /* skip unsuitable residue */
                continue;
            }

            if (is_prime_big(cand)) {
                /* Extend the cache if it is full. */
                if (primes_len == primes_cap) {
                    size_t new_cap = primes_cap ? (primes_cap << 1) : 8;
                    seq_t *tmp = (seq_t *)realloc(primes, new_cap * sizeof(seq_t));
                    if (!tmp) abort();      /* allocation failure */
                    primes = tmp;
                    primes_cap = new_cap;
                }
                primes[primes_len++] = cand;   /* store the new prime */
                break;                         /* move on to generate next */
            }
            ++cand;                    /* try next integer */
        }
    }

    return primes[n - 1];
}
