/*
 * A076293  Numbers k where the root mean square (RMS) of k and 7 is an
 *          integer, i.e., sqrt((k^2 + 7^2)/2) is an integer.
 * https://oeis.org/A076293
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 16, u32 n <= 34, u64 n <= 72, u128 n <= 148,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A076293: numbers k where sqrt((k^2 + 7^2)/2) is an integer.
   Recurrence: a(n) = 6*a(n-3) - a(n-6)
   Initial values: a(0)=1, a(1)=7, a(2)=17, a(3)=23, a(4)=49, a(5)=103 */
#include <stdlib.h>
#include <stddef.h>

static seq_t *cache = NULL;      /* array of computed terms */
static size_t  cache_cnt = 0;    /* number of terms stored (size of cache) */

seq_t a(idx_t n)
{
    /* create or grow cache to hold terms up to n */
    if (cache_cnt <= (size_t)n) {
        size_t new_cnt = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cnt * sizeof(seq_t));
        if (!tmp) abort();      /* no memory */
        cache = tmp;
        /* fill new positions */
        for (size_t i = cache_cnt; i <= (size_t)n; ++i) {
            seq_t val;
            if (i==0)      val = 1;
            else if (i==1) val = 7;
            else if (i==2) val = 17;
            else if (i==3) val = 23;
            else if (i==4) val = 49;
            else if (i==5) val = 103;
            else           val = 6*cache[i-3] - cache[i-6];
            cache[i] = val;
        }
        cache_cnt = new_cnt;
    }
    return cache[(size_t)n];
}
