/*
 * A163654  The sum of the n-th and (n+1)st isolated prime.
 * https://oeis.org/A163654
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = isolated_prime(n) + isolated_prime(n+1) Using the
 *            definition of isolated prime: p is a prime and p-2 and p+2 are
 *            composite. The function builds the list of isolated primes
 *            incrementally and returns the required sum
 * Range:     u8 n <= 11, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime, next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/is_prime.h"   /* bool is_prime(idx_t n) */
#include "../utils/next_prime.h"/* idx_t next_prime(idx_t n) */

seq_t a(idx_t n)
{
    /* static cache of isolated primes (1‑based indexing) */
    static idx_t *isolated = NULL;
    static size_t isolated_cap = 0;
    static size_t isolated_cnt = 0;
    static idx_t next_start = 3;   /* start searching after the prime 2 */

    /* Initialise cache with the trivial isolated prime 2 */
    if (isolated_cnt == 0) {
        isolated = (idx_t *)malloc(sizeof(idx_t));
        if (!isolated) return (seq_t)0; /* allocation failure -> zero */
        isolated[0] = 2;
        isolated_cnt = 1;
        isolated_cap = 1;
    }

    /* Ensure we have at least n+1 isolated primes needed for a(n) */
    while (isolated_cnt < (size_t)n + 1U) {
        idx_t p = next_prime(next_start);
        next_start = p + 1;                     /* next search starts after this prime */

        bool isolated_p = false;
        if (p == 2) {
            /* 2 is isolated (handled earlier but kept for safety) */
            isolated_p = true;
        } else if (p == 3) {
            /* 3 is not isolated: 5 is prime */
            isolated_p = false;
        } else {
            /* For primes > 3, check that both p-2 and p+2 are composite */
            if (!is_prime(p - 2) && !is_prime(p + 2))
                isolated_p = true;
        }

        /* Store isolated prime if found */
        if (isolated_p) {
            if (isolated_cnt >= isolated_cap) {
                size_t new_cap = (isolated_cap) ? isolated_cap * 2 : 128;
                idx_t *tmp = (idx_t *)realloc(isolated, new_cap * sizeof(idx_t));
                if (!tmp) {
                    free(isolated);
                    isolated   = NULL;
                    isolated_cap = 0;
                    isolated_cnt  = 0;
                    return (seq_t)0;
                }
                isolated = tmp;
                isolated_cap = new_cap;
            }
            isolated[isolated_cnt++] = p;
        }
    }

    /* Compute the sum of the n‑th and (n+1)st isolated primes (1‑based) */
    seq_t sum = (seq_t)isolated[(size_t)(n - 1)] + (seq_t)isolated[(size_t)n];
    return sum;
}
