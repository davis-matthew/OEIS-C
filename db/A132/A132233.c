/*
 * A132233  Primes congruent to 13 (mod 30).
 * https://oeis.org/A132233
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the nth prime congruent to 13 (mod 30)
 * Range:     u8 n <= 7, u16 n <= 820, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 62 terms through n = 970 (2026-11-03)
 * Helpers:   is_prime, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime.h"
#include "../utils/mem.h"

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;  /* offset starts at 1 */

    /* static cache of found primes */
    static seq_t *cache = NULL;
    static idx_t len   = 0;
    static idx_t cap   = 0;
    static idx_t nextCandidate = 13;  /* candidate to test, starts at first prime */

    if (len < n) {
        /* first allocation */
        if (cache == NULL) {
            cap = 16;
            cache = mem_new(seq_t, cap);
            if (cache == NULL) return (seq_t)0;  /* allocation failure */
        }

        /* extend cache until it contains at least n primes */
        while (len < n) {
            /* find next prime in the 13 (mod 30) progression */
            while (!is_prime(nextCandidate)) {
                nextCandidate += 30;
            }

            /* store prime */
            cache[len++] = (seq_t)nextCandidate;

            /* prepare next candidate */
            nextCandidate += 30;

            /* grow cache if needed */
            if (len == cap) {
                cap *= 2;
                seq_t *tmp = mem_resize(seq_t, cache, cap);
                if (tmp == NULL) break;  /* keep current cache */
                cache = tmp;
            }
        }
    }

    return cache[n - 1];
}
