/*
 * A081374  Size of "uniform" Hamming covers of distance 1, that is, Hamming
 *          covers in which all vectors of equal weight are treated the same,
 *          included or excluded from the cover together.
 * https://oeis.org/A081374
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*a(n-1) - a(n-3) + 2*a(n-4) for n ≥ 5 with a(1)=1,
 *            a(2)=2, a(3)=2, a(4)=5. This recurrence reproduces A081374. The
 *            implementation uses a static cache to allow arbitrary order of
 *            calls and grows the cache on demand
 * Range:     u8 n <= 9, u16 n <= 17, u32 n <= 33, u64 n <= 65, u128 n <= 129,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache: cache[i] contains a(i) for i>=1, cache[0] unused */
    static seq_t *cache = NULL;
    static size_t cap = 0;   /* allocated size */
    static size_t len = 0;   /* number of terms already computed (max i) */

    if (n == 0)
        return (seq_t)0;    /* undefined, but return 0 */

    /* grow cache if needed */
    if ((size_t)n >= cap) {
        size_t newcap = cap ? cap << 1 : 16;
        while (newcap <= (size_t)n)
            newcap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
        if (!tmp) return (seq_t)0; /* allocation failed */
        cache = tmp;
        cap = newcap;
    }

    /* compute any missing terms up to n */
    for (size_t i = len + 1; i <= (size_t)n; ++i) {
        seq_t val;
        if (i == 1)      val = (seq_t)1;
        else if (i == 2) val = (seq_t)2;
        else if (i == 3) val = (seq_t)2;
        else if (i == 4) val = (seq_t)5;
        else {
            val = 2 * cache[i-1];
            val -= cache[i-3];
            val += 2 * cache[i-4];
        }
        cache[i] = val;
    }

    len = (size_t)n;
    return cache[n];
}
