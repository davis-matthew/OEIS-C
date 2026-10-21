/*
 * A224727  Number of (n+3) X 10 0..2 matrices with each 4 X 4 subblock
 *          idempotent.
 * https://oeis.org/A224727
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 14, u32 n <= 49, u64 n <= 100, u128 n <= 100,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A224727 */
/* Empirical recurrence (valid for n > 10):
   a(n) = 3*a(n-1) - 2*a(n-2) - 2*a(n-3)
          + 4*a(n-4) - 3*a(n-5) + 2*a(n-7) - a(n-8)
   Initial values (n = 1..10):
   a(1) = 8117
   a(2) = 5711
   a(3) = 6540
   a(4) = 7132
   a(5) = 7575
   a(6) = 9090
   a(7) = 11268
   a(8) = 13821
   a(9) = 16593
   a(10) = 20653
*/
#include <stdlib.h>

static const seq_t init[11] = {
    (seq_t)0,            /* dummy for 0-index */
    (seq_t)8117,         /* a(1)  */
    (seq_t)5711,         /* a(2)  */
    (seq_t)6540,         /* a(3)  */
    (seq_t)7132,         /* a(4)  */
    (seq_t)7575,         /* a(5)  */
    (seq_t)9090,         /* a(6)  */
    (seq_t)11268,        /* a(7)  */
    (seq_t)13821,        /* a(8)  */
    (seq_t)16593,        /* a(9)  */
    (seq_t)20653         /* a(10) */
};

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;      /* cache[0] holds a(1) */
    static idx_t cache_size = 0;     /* number of items stored in cache */

    if (n < 1) return (seq_t)0;

    if (n > cache_size) {
        /* enlarge cache to hold values up to n */
        idx_t needed = n;
        seq_t *new_cache = (seq_t *)realloc(cache,
                                            sizeof(seq_t) * (size_t)needed);
        if (!new_cache) abort();      /* allocation failed */
        cache = new_cache;

        /* fill new entries */
        for (idx_t i = cache_size + 1; i <= needed; ++i) {
            if (i <= 10) {
                cache[i - 1] = init[i];
            } else {
                /* recurrence: a(i) = 3*a(i-1) - 2*a(i-2) - 2*a(i-3)
                                 + 4*a(i-4) - 3*a(i-5) + 2*a(i-7) - a(i-8) */
                seq_t val = (seq_t)3 * cache[i - 2]
                          + (seq_t)4 * cache[i - 5]
                          + (seq_t)2 * cache[i - 8]
                          - (seq_t)2 * cache[i - 3]
                          - (seq_t)2 * cache[i - 4]
                          - (seq_t)3 * cache[i - 6]
                          - cache[i - 9];
                cache[i - 1] = val;
            }
        }
        cache_size = needed;
    }
    return cache[n - 1];
}
