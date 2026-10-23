/*
 * A042543  Denominators of continued fraction convergents to sqrt(800).
 * https://oeis.org/A042543
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 8, u32 n <= 16, u64 n <= 35, u128 n <= 67,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A042543
   a(n) = 39202*a(n-8) - a(n-16)   (n > 15)
   a(0)=1, a(1)=3, a(2)=4, a(3)=7, a(4)=95, a(5)=102,
   a(6)=197, a(7)=693, a(8)=39005, a(9)=117708,
   a(10)=156713, a(11)=274421, a(12)=3724186,
   a(13)=3998607, a(14)=7722793, a(15)=27166986
*/

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache      = NULL;
    static size_t cache_size = 0;
    static size_t cache_cap  = 0;

    /* first use: initialise the base 16 terms */
    if (cache_size == 0) {
        cache_cap = 16;
        cache = (seq_t *)realloc(NULL, cache_cap * sizeof(seq_t));
        cache[0 ] = (seq_t)1;
        cache[1 ] = (seq_t)3;
        cache[2 ] = (seq_t)4;
        cache[3 ] = (seq_t)7;
        cache[4 ] = (seq_t)95;
        cache[5 ] = (seq_t)102;
        cache[6 ] = (seq_t)197;
        cache[7 ] = (seq_t)693;
        cache[8 ] = (seq_t)39005;
        cache[9 ] = (seq_t)117708;
        cache[10] = (seq_t)156713;
        cache[11] = (seq_t)274421;
        cache[12] = (seq_t)3724186;
        cache[13] = (seq_t)3998607;
        cache[14] = (seq_t)7722793;
        cache[15] = (seq_t)27166986;
        cache_size = 16;
    }

    size_t idx = (size_t)n;
    if (idx < cache_size) {
        return cache[idx];
    }

    /* grow capacity if needed */
    if (cache_cap <= idx) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 16;
        while (new_cap <= idx) {
            new_cap <<= 1;
        }
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) { /* allocation failed – return 0 as fallback */
            return (seq_t)0;
        }
        cache = tmp;
        cache_cap = new_cap;
    }

    /* compute missing terms using the recurrence */
    for (size_t i = cache_size; i <= idx; ++i) {
        cache[i] = (seq_t)39202 * cache[i - 8] - cache[i - 16];
    }
    cache_size = idx + 1;

    return cache[idx];
}
