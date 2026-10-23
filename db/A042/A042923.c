/*
 * A042923  Denominators of continued fraction convergents to sqrt(993).
 * https://oeis.org/A042923
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 17, u64 n <= 32, u128 n <= 62,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A042923
   Denominators of continued fraction convergents to sqrt(993).
   Recurrence: a(n) = 5294*a(n-6) - a(n-12) for n > 11
   Initial values:
     a(0)=1, a(1)=1, a(2)=2, a(3)=41, a(4)=43, a(5)=84,
     a(6)=5251, a(7)=5335, a(8)=10586, a(9)=217055,
     a(10)=227641, a(11)=444696
*/
#include <stddef.h>
#include <stdlib.h>

static seq_t *cache   = NULL;
static size_t cache_cap = 0;
static size_t cache_len = 0;

seq_t a(idx_t n)
{
    /* Ensure the cache can hold up to index n. */
    if ((size_t)n >= cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 16;
        while (new_cap <= (size_t)n) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();   /* Out of memory – abort as no error reporting is defined. */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Compute missing terms up to n. */
    for (size_t i = cache_len; i <= (size_t)n; ++i) {
        if (i <= 11) {
            /* Initial values. */
            static const seq_t init[12] = {
                (seq_t)1, (seq_t)1, (seq_t)2, (seq_t)41,
                (seq_t)43, (seq_t)84, (seq_t)5251, (seq_t)5335,
                (seq_t)10586, (seq_t)217055, (seq_t)227641,
                (seq_t)444696
            };
            cache[i] = init[i];
        } else {
            seq_t t = cache[i - 6];
            t *= (seq_t)5294;
            t -= cache[i - 12];
            cache[i] = t;
        }
    }

    cache_len = (size_t)n + 1;
    return cache[(size_t)n];
}
