/*
 * A126725  a(1)=0, a(2)=1; for n>2, a(n) = C(n,2)*(1+a(n-2)).
 * https://oeis.org/A126725
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = C(n,2)*(1 + a(n-2)), a(1)=0, a(2)=1
 * Range:     u8 n <= 6, u16 n <= 9, u32 n <= 14, u64 n <= 22, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache to handle arbitrary call order */
    static seq_t *cache   = NULL;
    static size_t cap     = 0;      /* allocated size of cache */
    static idx_t max_n    = 0;      /* highest n that has been computed */

    /* ensure enough space for index n (1‑based) */
    if ((size_t)(n + 1) > cap) {
        size_t newcap = (cap == 0) ? 4 : cap;
        while (newcap < (size_t)(n + 1))
            newcap <<= 1;
        cache = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
        cap   = newcap;
    }

    /* initialize base cases if not already done */
    if (max_n < 1) {
        cache[1] = (seq_t)0;
        max_n = 1;
    }
    if (max_n < 2) {
        cache[2] = (seq_t)1;
        max_n = 2;
    }

    /* compute missing values up to n */
    if (n > max_n) {
        for (idx_t i = max_n + 1; i <= n; ++i) {
            seq_t prev2 = cache[i - 2];
            seq_t bin   = (seq_t)i * ((seq_t)i - (seq_t)1) / (seq_t)2;
            cache[i]    = bin * ((seq_t)1 + prev2);
        }
        max_n = n;
    }

    return cache[n];
}
