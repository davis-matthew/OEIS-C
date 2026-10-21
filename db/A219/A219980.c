/*
 * A219980  Unmatched value maps: number of nX3 binary arrays indicating the
 *          locations of corresponding elements not equal to any horizontal,
 *          vertical or antidiagonal neighbor in a random 0..3 nX3 array.
 * https://oeis.org/A219980
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 8*a(n-1)-7*a(n-2)+27*a(n-3)-23*a(n-4)+28*a(n-5)-18*a(n-6)
 *            -5*a(n-7)+10*a(n-8)-a(n-9)-3*a(n-10)+a(n-11), with a(1)=4,
 *            a(2)=42, a(3)=316, a(4)=2344, a(5)=17591, a(6)=131978,
 *            a(7)=989813, a(8)=7423785, a(9)=55680172, a(10)=417613528,
 *            a(11)=3132193649
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 22, u128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Handle out‑of‑range indices (OEIS offset is 1). */
    if (n < 1)
        return (seq_t)0;

    /* Static cache to memoise computed values */
    static seq_t *cache = NULL;
    static size_t cap = 0;        /* allocated capacity (indices 0..cap) */
    static idx_t highest = 0;     /* largest n already computed */

    /* Initialise cache the first time */
    if (cache == NULL) {
        cap = 16; /* start with space for at least the first 11 terms */
        cache = (seq_t *)malloc((cap + 1) * sizeof(seq_t));
        if (!cache)
            return (seq_t)0;   /* allocation failed */

        /* Base terms a(1)…a(11) */
        cache[1]  = (seq_t)4;
        cache[2]  = (seq_t)42;
        cache[3]  = (seq_t)316;
        cache[4]  = (seq_t)2344;
        cache[5]  = (seq_t)17591;
        cache[6]  = (seq_t)131978;
        cache[7]  = (seq_t)989813;
        cache[8]  = (seq_t)7423785;
        cache[9]  = (seq_t)55680172;
        cache[10] = (seq_t)417613528;
        cache[11] = (seq_t)3132193649;
        highest   = 11;
    }

    /* Resize cache if necessary */
    if (n > cap) {
        size_t new_cap = cap;
        while (n > new_cap)
            new_cap <<= 1;          /* double until it fits */
        seq_t *tmp = (seq_t *)realloc(cache, (new_cap + 1) * sizeof(seq_t));
        if (!tmp)
            return (seq_t)0;        /* allocation failed */
        cache = tmp;
        cap   = new_cap;
    }

    /* Return cached value if already known */
    if (n <= highest)
        return cache[n];

    /* Compute missing terms up to n */
    for (idx_t i = highest + 1; i <= n; ++i) {
        seq_t term = 0;
        term += (seq_t)8  * cache[i-1];
        term -= (seq_t)7  * cache[i-2];
        term += (seq_t)27 * cache[i-3];
        term -= (seq_t)23 * cache[i-4];
        term += (seq_t)28 * cache[i-5];
        term -= (seq_t)18 * cache[i-6];
        term -= (seq_t)5  * cache[i-7];
        term += (seq_t)10 * cache[i-8];
        term -=          cache[i-9];
        term -= (seq_t)3  * cache[i-10];
        term +=          cache[i-11];
        cache[i] = term;
    }

    highest = n;
    return cache[n];
}
