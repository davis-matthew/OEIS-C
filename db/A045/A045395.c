/*
 * A045395  Primes congruent to {3, 5, 7} mod 8.
 * https://oeis.org/A045395
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 43, u16 n <= 4927, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the nth prime that is congruent to 3, 5, or 7 (mod 8). */
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;                      /* undefined for n < 1 */

    /* Cache for previously found qualifying primes. */
    static seq_t *cache = NULL;                /* array of found primes */
    static idx_t cache_cnt = 0;                /* number of primes stored   */
    static idx_t cache_cap = 0;                /* allocated capacity        */
    static idx_t next_candidate = 3;           /* candidate to test next    */

    /* Generate until we have at least n primes. */
    while (cache_cnt < n) {
        for (idx_t p = next_candidate; ; p += 2) {
            /* test only odd numbers starting from 3 */
            if (is_prime(p) &&
                ((p & 7) == 3 || (p & 7) == 5 || (p & 7) == 7)) {

                /* Resize cache if necessary. */
                if (cache_cnt == cache_cap) {
                    idx_t new_cap = (cache_cap == 0) ? 64 : cache_cap * 2;
                    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                    if (!tmp) return 0;           /* allocation failure */
                    cache = tmp;
                    cache_cap = new_cap;
                }

                /* Store the prime (truncated if it exceeds seq_t). */
                cache[cache_cnt++] = (seq_t)p;

                /* Next candidate starts after the found prime. */
                next_candidate = p + 2;
                break;
            }
        }
    }

    return cache[n - 1];
}
