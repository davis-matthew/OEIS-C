/*
 * A145720  Numbers x such that there exists n in N with (x+1)^3-x^3=127*n^2.
 * https://oeis.org/A145720
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n+2) = 2030*a(n+1) - a(n) + 1014, a(1)=6, a(2)=13201
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 3, u64 n <= 6, u128 n <= 12,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static memoisation of previously computed terms */
    static seq_t *cache = NULL;   /* cache[0] unused, indices [1..cached] valid */
    static idx_t cached = 0;

    if (n == 0)
        return (seq_t)0;  /* undefined for offset 1 */

    /* ensure enough space in cache; use explicit cast to satisfy C++ */
    if (n > cached) {
        idx_t newsize = n + 1; /* include index 0 which stays unused */
        seq_t *p = (seq_t *)realloc(cache, newsize * sizeof(seq_t));
        if (!p)
            return (seq_t)0;  /* allocation failure */
        cache = p;

        /* compute new entries */
        for (idx_t i = cached + 1; i <= n; ++i) {
            if (i == 1) {
                cache[1] = (seq_t)6;
            } else if (i == 2) {
                cache[2] = (seq_t)13201;
            } else {
                /* use 128‑bit intermediate when seq_t fits in 64‑bits */
                if (sizeof(seq_t) <= sizeof(unsigned long long)) {
                    unsigned __int128 prod = (unsigned __int128)cache[i - 1] * 2030;
                    cache[i] = (seq_t)(prod - cache[i - 2] + 1014);
                } else {
                    cache[i] = cache[i - 1] * (seq_t)2030 -
                               cache[i - 2] + (seq_t)1014;
                }
            }
        }
        cached = n;
    }

    return cache[n];
}
