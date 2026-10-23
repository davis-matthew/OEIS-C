/*
 * A180958  Diagonal sums of generalized Narayana triangle A180957.
 * https://oeis.org/A180958
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign
 * Formula:   a(n) = 2*a(n-1) + a(n-2) - 3*a(n-3) - a(n-4), a(0) = 1, a(1) =
 *            1, a(2) = 2, a(3) = 2
 * Range:     i8 n <= 9, i16 n <= 19, i32 n <= 38, i64 n <= 78, i128 n <= 159,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;          /* static memoization cache   */
static size_t cache_len = 0;         /* number of cached entries    */

static void ensure_cache(idx_t n)
{
    size_t needed = (size_t)n + 1;
    if (cache_len >= needed) return;              /* already cached */

    seq_t *new_cache = (seq_t *)realloc(cache, needed * sizeof(seq_t));
    if (!new_cache) abort();                      /* allocation failure */
    cache = new_cache;

    /* initialise initial four terms if they are not yet present   */
    for (size_t i = cache_len; i < needed && i < (size_t)4; ++i) {
        switch (i) {
            case 0: cache[0] = (seq_t)1; break;
            case 1: cache[1] = (seq_t)1; break;
            case 2: cache[2] = (seq_t)2; break;
            case 3: cache[3] = (seq_t)2; break;
        }
    }

    /* compute following terms using the linear recurrence              */
    for (size_t i = 4; i < needed; ++i) {
        seq_t a1 = cache[i-1];
        seq_t a2 = cache[i-2];
        seq_t a3 = cache[i-3];
        seq_t a4 = cache[i-4];
        cache[i] = a1 * (seq_t)2 + a2 - a3 * (seq_t)3 - a4;
    }

    cache_len = needed;
}

seq_t a(idx_t n)
{
    ensure_cache(n);
    return cache[(size_t)n];
}
