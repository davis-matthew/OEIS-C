/*
 * A052985  Expansion of ( 1-x ) / ( 1-3*x+x^2-x^3+x^4 ).
 * https://oeis.org/A052985
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 22, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence:
   a(0)=1, a(1)=2, a(2)=5, a(3)=14
   a(n) = 3*a(n-1) - a(n-2) + a(n-3) - a(n-4)   for n≥4   */
#include <stddef.h>
#include <stdlib.h>

static seq_t *cache = NULL;      /* memoization array            */
static size_t  cap   = 0;        /* allocated capacity           */
static idx_t   next  = 0;        /* first index not yet computed */

static void ensure(idx_t n)
{
    /* grow the cache array if needed */
    if (cache == NULL) {
        cap = 16;
        cache = (seq_t *)malloc(sizeof(seq_t) * cap);
    }
    while (cap <= (size_t)n) {
        cap *= 2;
        cache = (seq_t *)realloc(cache, sizeof(seq_t) * cap);
    }
    /* compute missing terms */
    while (next <= n) {
        seq_t val;
        if (next == 0)           val = 1;
        else if (next == 1)      val = 2;
        else if (next == 2)      val = 5;
        else if (next == 3)      val = 14;
        else {
            /* a(n) = 3*a(n-1) - a(n-2) + a(n-3) - a(n-4) */
            val = cache[next-1] + cache[next-1] + cache[next-1]; /* 3*a(n-1) */
            val -= cache[next-2];            /* - a(n-2) */
            val += cache[next-3];            /* + a(n-3) */
            val -= cache[next-4];            /* - a(n-4) */
        }
        cache[next] = val;
        ++next;
    }
}

seq_t a(idx_t n)
{
    ensure(n);
    return cache[n];
}
