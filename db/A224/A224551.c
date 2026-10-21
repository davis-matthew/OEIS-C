/*
 * A224551  Number of (n+2)X(n+2) 0..1 matrices with each 3X3 subblock
 *          idempotent.
 * https://oeis.org/A224551
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   Empirical recurrence a(n) =
 *            6*a(n-1)-15*a(n-2)+23*a(n-3)-30*a(n-4)+36*a(n-5)
 *            -34*a(n-6)+27*a(n-7)-21*a(n-8)+13*a(n-9)-6*a(n-10)+3*a(n-11)-a(n-12)
 *            Initial values : a(1)=50, a(2)=94, a(3)=180, a(4)=286, a(5)=540,
 *            a(6)=992, a(7)=1668, a(8)=2852, a(9)=4870, a(10)=8086,
 *            a(11)=13318, a(12)=21860, a(13)...
 * Range:     u8 n <= 3, u16 n <= 14, u32 n <= 28, u64 n <= 28, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static idx_t cached_max = 0;

    if (n < 1) return 0;  /* undefined for n<1 */

    /* initialisation */
    if (cache == NULL) {
        /* ensure we have space for indices 0..14 */
        cache_cap = (n + 1 > 15) ? (n + 1) : 15;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) return 0; /* allocation failed */

        static const seq_t init[15] = {
            0, 50, 94, 180, 286, 540, 992, 1668,
            2852, 4870, 8086, 13318, 21860, 35536, 57364
        };
        size_t limit = (cache_cap - 1 < 14) ? (cache_cap - 1) : 14;
        for (idx_t i = 1; i <= limit; ++i) {
            cache[i] = init[i];
        }
        cached_max = limit;  /* may be <14 if n < 14 */
    }
    /* grow cache if necessary */
    else if (n > cached_max) {
        size_t new_cap = n + 1;
        if (new_cap > cache_cap) {
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (tmp) cache = tmp;
            cache_cap = new_cap;
        }
    }

    if (n <= cached_max)
        return cache[n];

    /* compute from cached_max+1 up to n */
    for (idx_t i = cached_max + 1; i <= n; ++i) {
        if (i <= 14) {
            /* already initialised during allocation */
            continue;
        }

        seq_t pos = 0;
        pos += cache[i - 1] * (seq_t)6;
        pos += cache[i - 3] * (seq_t)23;
        pos += cache[i - 5] * (seq_t)36;
        pos += cache[i - 7] * (seq_t)27;
        pos += cache[i - 9] * (seq_t)13;
        pos += cache[i - 11] * (seq_t)3;

        seq_t sub = 0;
        sub += cache[i - 2] * (seq_t)15;
        sub += cache[i - 4] * (seq_t)30;
        sub += cache[i - 6] * (seq_t)34;
        sub += cache[i - 8] * (seq_t)21;
        sub += cache[i - 10] * (seq_t)6;
        sub += cache[i - 12];

        cache[i] = pos - sub;
    }

    cached_max = n;
    return cache[n];
}
