/*
 * A145336  Numbers x such that there exists n in N : (x+1)^3-x^3=43*n^2.
 * https://oeis.org/A145336
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n+2) = 33710*a(n+1) - a(n) + 16854; Initial values: a(1) = 26,
 *            a(2) = 893341
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 2, u64 n <= 4, u128 n <= 9,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;       /* stored terms, 1‑based index => [n‑1] */
    static size_t count      = 0;     /* highest index currently stored   */
    static size_t capacity   = 0;     /* allocated capacity of cache      */

    if (n == 0) return (seq_t)0;      /* undefined for offset < 1, but harmless */

    /* Return if already computed */
    if ((size_t)n <= count)
        return cache[(size_t)n - 1];

    /* Grow cache to at least n */
    size_t newcap = capacity ? capacity * 2 : 16;
    while (newcap < (size_t)n) newcap <<= 1;
    if (newcap != capacity) {
        seq_t *tmp = (seq_t*)realloc(cache, newcap * sizeof(seq_t));
        cache = tmp;
        capacity = newcap;
    }

    /* Compute missing terms from count+1 up to n */
    for (size_t i = count + 1; i <= (size_t)n; ++i) {
        if (i == 1)
            cache[0] = (seq_t)26;
        else if (i == 2)
            cache[1] = (seq_t)893341;
        else {
            seq_t a_nm2 = cache[i - 3];    /* a(i-2)  */
            seq_t a_nm1 = cache[i - 2];    /* a(i-1)  */
            cache[i - 1] = a_nm1 * (seq_t)33710 - a_nm2 + (seq_t)16854;
        }
    }
    count = (size_t)n;
    return cache[(size_t)n - 1];
}
