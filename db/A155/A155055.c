/*
 * A155055  Primes without positive even digits.
 * https://oeis.org/A155055
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the n-th prime containing no positive even digits
 *            (2,4,6,8) in its decimal representation
 * Range:     u8 n <= 31, u16 n <= 916, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime.h"  /* provides bool is_prime(idx_t x) */

seq_t a(idx_t n) {
    /* static state to cache primes and to avoid recomputing from scratch */
    static seq_t *cache = NULL;
    static size_t cap = 0;       /* allocated capacity */
    static size_t cached = 0;   /* number of primes stored */
    static idx_t last_checked = 1; /* last number examined */

    if (n == 0) return (seq_t)0; /* undefined for n<1, but return 0 safely */

    /* extend cache until we have at least n primes */
    while (cached < n) {
        last_checked++;  /* check next integer */

        if (!is_prime(last_checked))
            continue;   /* skip non-primes */

        /* Check that none of the decimal digits is 2,4,6,8 */
        uint64_t tmp = (uint64_t)last_checked;
        bool ok = true;
        while (tmp > 0) {
            uint8_t d = (uint8_t)(tmp % 10);
            if (d % 2 == 0 && d != 0) { /* positive even digit found */
                ok = false;
                break;
            }
            tmp /= 10;
        }
        if (!ok) continue; /* digit constraint failed */

        /* store the prime in the cache */
        if (cached == cap) {
            size_t new_cap = cap ? cap * 2 : 8;
            seq_t *temp = (seq_t *)realloc(cache, sizeof(seq_t) * new_cap);
            if (temp == NULL) { /* allocation failed, abort */
                exit(EXIT_FAILURE);
            }
            cache = temp;
            cap = new_cap;
        }
        cache[cached++] = (seq_t)last_checked;
    }

    return cache[n - 1];
}
