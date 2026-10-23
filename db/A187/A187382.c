/*
 * A187382  Number of 8-step S, NW and NE-moving king's tours on an n X n
 *          board summed over all starting positions.
 * https://oeis.org/A187382
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3*a(n‑1) - 3*a(n‑2) + a(n‑3) for n > 9 with initial
 *            values: a(1)=0, a(2)=0, a(3)=0, a(4)=404, a(5)=2140, a(6)=6380,
 *            a(7)=13220, a(8)=22996, a(9)=35366
 * Range:     u8 n <= 3, u16 n <= 10, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n < 1)
        return 0;

    /* static cache of computed values, indexed from 1 */
    static seq_t *cache = NULL;
    static idx_t cache_cap = 0;    /* largest n for which cache[1..n] are valid */

    if (n > cache_cap) {
        /* enlarge cache to at least n */
        idx_t new_cap = n;
        if (new_cap < 10)             /* keep some space */
            new_cap = 10;

        seq_t *new_cache = (seq_t *)realloc(cache, (new_cap + 1) * sizeof(seq_t));
        if (!new_cache) {
            /* out of memory – fall back to simple computation (unlikely) */
            seq_t a1 = 0, a2 = 0, a3 = 0, a4 = 404, a5 = 2140, a6 = 6380;
            seq_t a7 = 13220, a8 = 22996, a9 = 35366;
            seq_t result = 0;
            if (n <= 3)      result = 0;
            else if (n == 4) result = a4;
            else if (n == 5) result = a5;
            else if (n == 6) result = a6;
            else if (n == 7) result = a7;
            else if (n == 8) result = a8;
            else if (n == 9) result = a9;
            else {
                seq_t b1 = a9, b2 = a8, b3 = a7;
                for (idx_t i = 10; i <= n; ++i) {
                    seq_t next = b1 + b1 + b1 - b2 - b2 - b2 + b3;
                    b3 = b2; b2 = b1; b1 = next;
                }
                result = b1;
            }
            return result;
        }
        cache = new_cache;
        /* fill new entries */
        for (idx_t i = cache_cap + 1; i <= new_cap; ++i) {
            if (i <= 3) {
                cache[i] = 0;
            } else if (i == 4) {
                cache[i] = 404;
            } else if (i == 5) {
                cache[i] = 2140;
            } else if (i == 6) {
                cache[i] = 6380;
            } else if (i == 7) {
                cache[i] = 13220;
            } else if (i == 8) {
                cache[i] = 22996;
            } else if (i == 9) {
                cache[i] = 35366;
            } else {
                seq_t a1 = cache[i - 1];
                seq_t a2 = cache[i - 2];
                seq_t a3 = cache[i - 3];
                /* compute a(i) = 3*a1 - 3*a2 + a3 safely */
                cache[i] = a1 + a1 + a1 - a2 - a2 - a2 + a3;
            }
        }
        cache_cap = new_cap;
    }
    return cache[n];
}
