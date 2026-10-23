/*
 * A142656  Primes congruent to 37 mod 56.
 * https://oeis.org/A142656
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   successive primes congruent to 37 mod 56; a(n) is the n‑th such
 *            prime
 * Range:     u8 n <= 2, u16 n <= 274, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n) {
    /* static cache of found primes */
    static seq_t *cache      = NULL;
    static idx_t  usize     = 0;   /* number of primes stored */
    static idx_t  cap       = 0;   /* current cache capacity */
    static idx_t  candidate = 37;  /* next number to test, of form 56k+37 */

    if (n == 0) return 0; /* offset 1; n≥1 expected */

    /* grow cache when needed */
    if (cache == NULL) {
        cap = 128;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
    }

    /* add primes until we have at least n of them */
    while (usize < n) {
        if (is_prime(candidate)) {
            if (usize == cap) {          /* double capacity */
                cap <<= 1;
                seq_t *tmp = (seq_t *)realloc(cache, cap * sizeof(seq_t));
                if (!tmp) {
                    /* allocation failed – abort (undefined behaviour) */
                    return 0;
                }
                cache = tmp;
            }
            cache[usize++] = (seq_t)candidate; /* store prime */
        }
        candidate += 56; /* move to the next number of the form 56k+37 */
    }

    return cache[n - 1]; /* 1‑based index */
}
