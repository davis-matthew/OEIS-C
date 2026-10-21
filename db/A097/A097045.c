/*
 * A097045  a(n) = a(n-1) - a(floor(sqrt(n))) for n > 1; a(1) = 1.
 * https://oeis.org/A097045
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = a(1) = 1; a(n) = a(n-1) - a(floor(sqrt(n))) for n > 1
 * Range:     i8 n <= 88, i16 n <= 88, i32 n <= 88, i64 n <= 88, i128 n <= 88,
 *            big any n
 * Helpers:   floor_isqrt_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/floor_isqrt_u64.h"

seq_t a(idx_t n)
{
    /* static cache[0] is unused; cache[1] .. cache[max_cache] contain computed values  */
    static seq_t *cache = NULL;   /* raw array of seq_t objects */
    static idx_t max_cache = 0;   /* highest n for which cache[n] is valid */

    /* Ensure the caller's n is non‑zero; a(0) is undefined and never requested. */
    if (n == 0) return (seq_t)0;  /* defensive, not part of the OEIS definition */

    /* Grow the cache if needed */
    if (n > max_cache) {
        idx_t new_size = n;   /* we want to store indices 1..n inclusive   */
        /* realloc returns void*; cast explicitly to seq_t*  */
        seq_t *tmp = (seq_t *)realloc(cache, (new_size + 1) * sizeof(seq_t));
        if (tmp == NULL) return (seq_t)0;   /* allocation failure; unlikely for small n */

        cache = tmp;
        /* Compute the new entries only; indices 1..max_cache are already correct */
        for (idx_t i = max_cache + 1; i <= new_size; ++i) {
            if (i == 1) {
                cache[1] = (seq_t)1;
            } else {
                idx_t rad = floor_isqrt_u64(i);       /* floor(sqrt(i))  */
                cache[i] = cache[i - 1] - cache[rad];
            }
        }
        max_cache = new_size;
    }
    return cache[n];
}
