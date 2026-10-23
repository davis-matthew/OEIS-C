/*
 * A129908  Quotient of the decimal representation of concatenated twin primes
 *          divided by 3.
 * https://oeis.org/A129908
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, frac, nonn
 * Formula:   a(n) = concatenated twin prime pair / 3
 * Range:     u8 n <= 2, u16 n <= 8, u32 n <= 745, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   concat_parts, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/is_prime.h"
#include "../utils/concat_parts.h"
#include <stdlib.h>

/* Computes the n‑th term of OEIS sequence A129908. */
seq_t a(idx_t n)
{
    /* Cached results for the sequence */
    static seq_t *cache = NULL;
    static size_t   cap   = 0;   /* current allocated capacity   */
    static size_t   len   = 0;   /* number of terms currently cached */
    static idx_t   next   = 3;   /* next prime candidate to examine */

    /* Fill the cache until we have at least n terms */
    while (len < n) {
        idx_t p = next;
        /* Skip even numbers (though next always starts odd) */
        if ((p & 1) == 0)
            ++p;

        /* Find the next twin‑prime pair */
        while (1) {
            if (is_prime(p) && is_prime(p + 2)) {
                /* Concatenate the twin primes and divide by 3 */
                seq_t val = concat_parts(p, p + 2);
                val /= (seq_t)3;

                /* Grow the cache if needed */
                if (len >= cap) {
                    size_t new_cap = cap ? cap * 2 : 4;
                    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                    if (!tmp) abort();                 /* allocation failure */
                    cache = tmp;
                    cap   = new_cap;
                }

                cache[len++] = val;          /* store the new term    */
                next = p + 2;                /* start searching after this pair */
                break;                       /* ready for next iteration */
            }
            p += 2;  /* jump to next odd candidate */
        }
    }
    return cache[(size_t)(n - 1)];
}
