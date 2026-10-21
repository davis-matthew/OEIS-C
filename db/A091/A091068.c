/*
 * A091068  a(0) = 0; for n>0, a(n) = a(n-1) - n if that is >= 0, else a(n) =
 *          a(n-1) + n - 1.
 * https://oeis.org/A091068
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 0; For n > 0, a(n) = a(n-1) - n if a(n-1) >= n, a(n-1) +
 *            n - 1 otherwise. The sequence is computed iteratively and cached
 *            for arbitrary call order
 * Range:     u8 n <= 205, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache: cache[i] holds a(i) */
    static seq_t *cache = NULL;      /* array of a(i)           */
    static size_t cap   = 0;         /* current allocation size */
    static idx_t   last = 0;         /* largest n already computed */

    /* first time: allocate one element for a(0) */
    if (cache == NULL) {
        cap   = 1;
        cache = (seq_t*)malloc(cap * sizeof(seq_t));
        cache[0] = (seq_t)0;   /* a(0) */
        last  = 0;
    }

    /* enlarge cache if needed */
    if ((size_t)n + 1 > cap) {
        size_t new_cap = cap == 0 ? 1 : cap << 1;
        while ((size_t)n + 1 > new_cap) new_cap <<= 1;
        cache = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        cap   = new_cap;
    }

    /* if already computed, return the value */
    if (n <= last) return cache[n];

    /* compute values from last+1 up to n */
    seq_t prev = cache[last];
    for (idx_t i = last + 1; i <= n; ++i) {
        if (prev >= (seq_t)i) {
            cache[i] = prev - (seq_t)i;
        } else {
            cache[i] = prev + (seq_t)i - 1;
        }
        prev = cache[i];
    }
    last = n;
    return cache[n];
}
