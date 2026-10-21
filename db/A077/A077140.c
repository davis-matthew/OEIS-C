/*
 * A077140  a(1) = 1 and then add n to the previous term if n is coprime to
 *          the previous term, otherwise subtract n from the previous term.
 *          a(1) = 1 and a(n) = a(n-1) + n if gcd(n, a(n-1)) = 1, otherwise
 *          a(n) = a(n-1) - n.
 * https://oeis.org/A077140
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, easy
 * Formula:   not stated
 * Range:     i8 n <= 128, i16 n <= 1000, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence:
   a(1) = 1
   a(n) = a(n-1) + n   if gcd(n, a(n-1)) == 1
   a(n) = a(n-1) - n   otherwise
*/
#include <stdlib.h>
#include "../utils/gcd.h"

seq_t a(idx_t n) {
    /* cache holds a(1)..a(cache_size) in cache[0]..[cache_size-1] */
    static seq_t *cache = NULL;
    static size_t cache_size = 0;

    if (n == 0) {
        return 0;   /* undefined for n<1 */
    }

    /* Compute missing terms up to n */
    while (cache_size < (size_t)n) {
        if (cache_size == 0) {
            cache = (seq_t*)malloc(sizeof(seq_t));
            if (!cache) return 0;         /* allocation failure */
            cache[0] = (seq_t)1;          /* a(1) */
            cache_size = 1;
        } else {
            seq_t prev = cache[cache_size - 1];
            seq_t step = (seq_t)(cache_size + 1); /* current n value */

            seq_t next;
            if (gcd(step, prev) == (seq_t)1) {
                next = prev + step;
            } else {
                next = prev - step;
            }

            seq_t *tmp = (seq_t*)realloc(cache, (cache_size + 1) * sizeof(seq_t));
            if (!tmp) return 0;           /* allocation failure */
            cache = tmp;
            cache[cache_size] = next;
            ++cache_size;
        }
    }

    return cache[n - 1];
}
