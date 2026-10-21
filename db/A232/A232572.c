/*
 * A232572  Number of (n+1)X(1+1) 0..2 arrays with every element next to
 *          itself plus and minus one within the range 0..2 horizontally,
 *          vertically or antidiagonally.
 * https://oeis.org/A232572
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*a(n-1) + 19*a(n-2) + 26*a(n-3) + 52*a(n-4) + 218*a(n-5)
 *            + 11*a(n-6) - 258*a(n-7) + 15*a(n-8) + 108*a(n-9) - 64*a(n-10) +
 *            16*a(n-11) Initial values: a(1)=16, a(2)=72, a(3)=516,
 *            a(4)=3048, a(5)=18944, a(6)=115892, a(7)=713488, a(8)=4389504,
 *            a(9)=26985196, a(10)=165981072, a(11)=1020610296
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 24, u128 n <= 48,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache that remembers computed terms */
    static seq_t *cache = NULL;
    static idx_t cache_cap = 0;     /* allocated size (max index + 1) */
    static idx_t max_computed = 0;  /* highest index actually computed */

    /* initial setup: allocate space for first 11 terms and store them */
    if (cache == NULL) {
        cache_cap = 11;
        cache = (seq_t*)calloc(cache_cap + 1, sizeof(seq_t)); /* index 0 unused */
        if (cache == NULL) return (seq_t)0; /* allocation failure, unlikely for n≤10000 */

        cache[1]  = (seq_t)16;
        cache[2]  = (seq_t)72;
        cache[3]  = (seq_t)516;
        cache[4]  = (seq_t)3048;
        cache[5]  = (seq_t)18944;
        cache[6]  = (seq_t)115892;
        cache[7]  = (seq_t)713488;
        cache[8]  = (seq_t)4389504;
        cache[9]  = (seq_t)26985196;
        cache[10] = (seq_t)165981072;
        cache[11] = (seq_t)1020610296;
        max_computed = 11;
    }

    /* ensure cache can hold up to index n */
    if (n > cache_cap) {
        idx_t new_cap = n;
        seq_t *tmp = (seq_t*)realloc(cache, sizeof(seq_t)*(new_cap + 1));
        if (tmp == NULL) return (seq_t)0; /* allocation failure */
        cache     = tmp;
        cache_cap = new_cap;
    }

    /* if requested term already computed, return it */
    if (n <= max_computed)
        return cache[n];

    /* compute terms from max_computed+1 up to n */
    for (idx_t i = max_computed + 1; i <= n; ++i) {
        seq_t value =
            (seq_t)2   * cache[i-1] +
            (seq_t)19  * cache[i-2] +
            (seq_t)26  * cache[i-3] +
            (seq_t)52  * cache[i-4] +
            (seq_t)218 * cache[i-5] +
            (seq_t)11  * cache[i-6]
            - (seq_t)258 * cache[i-7] +
            (seq_t)15  * cache[i-8] +
            (seq_t)108 * cache[i-9]
            - (seq_t)64  * cache[i-10] +
            (seq_t)16  * cache[i-11];
        cache[i] = value;
    }
    max_computed = n;
    return cache[n];
}
