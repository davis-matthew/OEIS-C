/*
 * A193653  Q-residue of the Delannoy triangle A008288, where Q is the
 *          triangular array (t(i,j)) given by t(i,j)=1.
 * https://oeis.org/A193653
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=1, a(1)=2, a(2)=6, a(n)=4*a(n-1)-a(n-2)-2*a(n-3) for n≥3
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 18, u64 n <= 35, u128 n <= 70,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>   /* for malloc/realloc */

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cap = 0;          /* allocated size of cache array   */
    static idx_t upto = 0;          /* highest n for which cache[n] is valid */

    /* initialise the cache with the first three terms */
    if (cache == NULL) {
        cap = 3;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (cache == NULL) return 0;   /* allocation failed – unlikely in tests */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)2;
        cache[2] = (seq_t)6;
        upto = 2;
    }

    /* if the requested n is already cached, return it immediately */
    if (n <= upto) return cache[n];

    /* extend the cache array if necessary */
    if ((size_t)n + 1 > cap) {
        size_t new_cap = cap * 2;
        if (new_cap < (size_t)n + 1) new_cap = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (tmp == NULL) return 0;   /* allocation failed */
        cache = tmp;
        cap = new_cap;
    }

    /* compute missing values in ascending order */
    for (idx_t i = upto + 1; i <= n; ++i) {
        seq_t val = cache[i-1];
        val = val * (seq_t)4;
        val -= cache[i-2];
        val -= cache[i-3] * (seq_t)2;
        cache[i] = val;
    }
    upto = n;
    return cache[n];
}
