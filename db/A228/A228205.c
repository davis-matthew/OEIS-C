/*
 * A228205  x-values in the solution to x^2 - 13y^2 = 108.
 * https://oeis.org/A228205
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 16, u32 n <= 34, u64 n <= 72, u128 n <= 146,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A228205:
   a(n) = 11*a(n-4) - a(n-8)
   with initial values
       a(1)=11, a(2)=15, a(3)=24, a(4)=41,
       a(5)=80, a(6)=141, a(7)=249, a(8)=440
*/

#include <stddef.h>
#include <stdlib.h>

/* Base values for indices 1..8 */
static const seq_t base_init[8] = {
    (seq_t)11, (seq_t)15, (seq_t)24, (seq_t)41,
    (seq_t)80, (seq_t)141, (seq_t)249, (seq_t)440
};

/* Cached terms, 0‑based index for a(i+1) */
static seq_t *cache = NULL;
static size_t cache_cap = 0;   /* allocated capacity */
static size_t cache_len = 0;   /* number of terms stored */

/* Ensure capacity for at least n terms (n is 1‑based). */
static void ensure_cache_up_to(idx_t n)
{
    size_t needed = (size_t)n;
    if (needed == 0) return;          /* no terms requested */

    /* Grow cache if necessary */
    if (cache_cap < needed) {
        size_t new_cap = cache_cap ? cache_cap << 1 : 8;
        while (new_cap < needed) new_cap <<= 1;

        seq_t *new_cache = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (!new_cache) abort();      /* allocation failure */

        cache = new_cache;
        cache_cap = new_cap;
    }

    /* Compute missing terms */
    for (size_t i = cache_len; i < needed; ++i) {
        if (i < 8) {
            cache[i] = base_init[i];
        } else {
            cache[i] = (seq_t)11 * cache[i - 4] - cache[i - 8];
        }
    }
    cache_len = needed;
}

/* Public function: a(idx_t n)  */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;         /* undefined for n=0, return 0 */
    ensure_cache_up_to(n);
    return cache[(size_t)n - 1];
}
