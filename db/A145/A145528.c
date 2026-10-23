/*
 * A145528  Numbers x such that (x+91)^3 - x^3 is a square.
 * https://oeis.org/A145528
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n+2) = 1454*a(n+1) - a(n) + 66066, a(1) = 455, a(2) = 728182
 * Range:     u8 none, u16 n <= 1, u32 n <= 3, u64 n <= 6, u128 n <= 12,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;      /* 1‑based array of computed terms */
static idx_t   cap   = 0;        /* largest index computed so far */

/* Compute the n‑th term of OEIS A145528. */
seq_t a(idx_t n)
{
    /* handle first two terms directly */
    if (n == 1) return (seq_t)455;
    if (n == 2) return (seq_t)728182;
    if (n < 1)   return (seq_t)0;          /* undefined for n<1 */

    /* ensure the cache exists and holds at least the first two terms */
    if (cap < 2) {
        size_t init = (n > 3 ? n + 1 : 4);   /* at least indices 0..3 */
        cache = (seq_t *)realloc(cache, init * sizeof(seq_t));
        cache[0] = 0;                       /* unused */
        cache[1] = (seq_t)455;
        cache[2] = (seq_t)728182;
        cap = 2;
    }

    /* grow the cache and compute missing terms */
    if (cap < n) {
        size_t newsize = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, newsize * sizeof(seq_t));
        if (!tmp) {                          /* allocation failure – abort */
            return (seq_t)0;
        }
        cache = tmp;
        for (idx_t i = cap + 1; i <= n; ++i) {
            cache[i] = ((seq_t)1454 * cache[i-1]) - cache[i-2] + (seq_t)66066;
        }
        cap = n;
    }

    return cache[n];
}
