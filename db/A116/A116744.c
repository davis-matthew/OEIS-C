/*
 * A116744  Number of permutations of length n which avoid the patterns 1243,
 *          1432, 4213.
 * https://oeis.org/A116744
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   linear recurrence a(n) = 8*a(n-1) - 26*a(n-2) + 47*a(n-3) -
 *            52*a(n-4) + 35*a(n-5) - 13*a(n-6) + 2*a(n-7) for n ≥ 8 Initial
 *            values: a(1)=1, a(2)=2, a(3)=6, a(4)=21, a(5)=73, a(6)=242,
 *            a(7)=772
 * Range:     u8 n <= 6, u16 n <= 10, u32 n <= 20, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t  max_comp = 0;   /* largest n that has been computed */
    static idx_t  capacity = 0;   /* allocated size of cache */

    /* initialize cache with the first seven values */
    if (capacity == 0) {
        capacity = 8;                               /* indexes 0..7 */
        cache = (seq_t*)malloc(sizeof(seq_t) * capacity);
        cache[0]  = 0; /* unused */
        cache[1]  = (seq_t)1;
        cache[2]  = (seq_t)2;
        cache[3]  = (seq_t)6;
        cache[4]  = (seq_t)21;
        cache[5]  = (seq_t)73;
        cache[6]  = (seq_t)242;
        cache[7]  = (seq_t)772;
        max_comp = 7;
    }

    /* grow cache if necessary */
    if (n >= capacity) {
        size_t new_cap = capacity * 2;
        if ((size_t)n + 1 > new_cap)
            new_cap = (size_t)n + 1;
        cache = (seq_t*)realloc(cache, sizeof(seq_t) * new_cap);
        capacity = new_cap;
    }

    /* return precomputed value if already available */
    if (n <= max_comp)
        return cache[n];

    /* compute missing values up to n */
    for (idx_t i = max_comp + 1; i <= n; ++i) {
        seq_t plus  = ((seq_t)8  * cache[i-1]) +
                      ((seq_t)47 * cache[i-3]) +
                      ((seq_t)35 * cache[i-5]) +
                      ((seq_t)2  * cache[i-7]);
        seq_t minus = ((seq_t)26 * cache[i-2]) +
                      ((seq_t)52 * cache[i-4]) +
                      ((seq_t)13 * cache[i-6]);
        cache[i] = plus - minus;
    }
    max_comp = n;
    return cache[n];
}
