/*
 * A063326  Dimension of the space of weight n cuspidal newforms for Gamma_1(
 *          53 ).
 * https://oeis.org/A063326
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  sign
 * Formula:   not stated
 * Range:     i8 n <= 3, i16 n <= 42, i32 n <= 42, i64 n <= 42, i128 n <= 42,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A063326
 *
 * a(n) = a(n-4) + a(n-6) - a(n-10)   for n > 13
 * with initial values (offset 2):
 *  a(2) = -1,  a(3) =  92,  a(4)  = 208,  a(5)  = 325,
 *  a(6) = 442,  a(7) = 559,  a(8)  = 676,  a(9)  = 793,
 *  a(10)= 910, a(11)=1027, a(12)=1144, a(13)=1259
 *
 * Behaviour for n < 2 is unspecified (returns 0).
 */
#include <stdlib.h>

static seq_t *cache = NULL;          /* memoisation array */
static idx_t  cache_size = 0;       /* number of cached entries */

/* initialise the cache up to n (inclusive) */
static void
ensure_cache(idx_t n)
{
    if (n + 1 <= cache_size)
        return;

    /* extend cache */
    seq_t *new_cache = (seq_t *)realloc(cache, sizeof(seq_t) * (n + 1));
    if (!new_cache) {
        /* allocation failed – abort program */
        abort();
    }
    cache = new_cache;

    /* base values for indices 0..13 */
    static const seq_t base[14] = {
        0,          /* unused */
        0,          /* unused */
        -1,         /* a(2) */
        92,         /* a(3) */
        208,        /* a(4) */
        325,        /* a(5) */
        442,        /* a(6) */
        559,        /* a(7) */
        676,        /* a(8) */
        793,        /* a(9) */
        910,        /* a(10) */
        1027,       /* a(11) */
        1144,       /* a(12) */
        1259        /* a(13) */
    };

    for (idx_t i = cache_size; i <= n; ++i) {
        if (i <= 13) {
            cache[i] = base[i];
        } else {
            /* recurrence: a(i) = a(i-4) + a(i-6) - a(i-10) */
            cache[i] = cache[i - 4] + cache[i - 6] - cache[i - 10];
        }
    }
    cache_size = n + 1;
}

seq_t
a(idx_t n)
{
    if (n < 2)
        return 0;               /* behaviour below offset is irrelevant */

    ensure_cache(n);
    return cache[n];
}
