/*
 * A204751  Number of (n+2) X 7 0..1 arrays with every 3 X 3 subblock having
 *          three equal elements in a row horizontally, vertically, diagonally
 *          or antidiagonally exactly three ways, and new values 0..1
 *          introduced in row major order.
 * https://oeis.org/A204751
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*a(n-1) - a(n-3) for n > 5 with initial values a(1)=342,
 *            a(2)=38, a(3)=35, a(4)=40, a(5)=48
 * Range:     u8 none, u16 n <= 21, u32 n <= 44, u64 n <= 90, u128 n <= 183,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* cache[0] holds a(1) */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;  /* number of terms cached (>=1 for a(1)) */

    /* safety for requests below the offset */
    if (n == 0)
        return (seq_t)0;

    /* expand cache if necessary */
    if (cache_len < n) {
        size_t new_len = (size_t)n;
        /* realloc returns void*, cast explicitly for C++ */
        seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        if (!tmp) abort();
        cache = tmp;

        /* compute missing terms */
        for (idx_t i = cache_len + 1; i <= n; ++i) {
            if (i <= 5) {
                /* initial terms (1‑based) */
                switch (i) {
                    case 1: cache[i-1] = (seq_t)342; break;
                    case 2: cache[i-1] = (seq_t)38;  break;
                    case 3: cache[i-1] = (seq_t)35;  break;
                    case 4: cache[i-1] = (seq_t)40;  break;
                    case 5: cache[i-1] = (seq_t)48;  break;
                }
            } else {
                /* a(i) = 2*a(i-1) - a(i-3) */
                seq_t val = cache[i-2] + cache[i-2];   /* 2*a(i-1)   */
                cache[i-1] = val - cache[i-4];         /* -a(i-3)    */
            }
        }
        cache_len = n;
    }

    return cache[n-1];
}
