/*
 * A215193  Number of arrays of 6 0..n integers with no sum of consecutive
 *          elements equal to a disjoint adjacent sum of an equal number of
 *          elements.
 * https://oeis.org/A215193
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*a(n-1) + a(n-2) - 2*a(n-3) - 2*a(n-4) - 2*a(n-5) +
 *            5*a(n-6) + 2*a(n-7) - 2*a(n-9) - 5*a(n-10) + 2*a(n-11) +
 *            2*a(n-12) + 2*a(n-13) - a(n-14) - 2*a(n-15) + a(n-16) Initial
 *            values (OEIS offset 1): a(1)=0, a(2)=30, a(3)=440, a(4)=2898,
 *            a(5)=11756, a(6)=36864, a(7)=95832, a(8)=219092, a(9)=45...
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 40, u64 n <= 135, u128 n <= 135,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
 
seq_t a(idx_t n) {
    /* static cache for computed values */
    static seq_t *cache = NULL;          /* array indexed from 0..max_n */
    static size_t capacity = 0;          /* allocated size */
    static idx_t max_computed = 0;       /* highest n computed */
    static bool initialized = false;    /* initial values set? */
 
    if (n < 1) return 0;   /* undefined below offset, return 0 */
 
    /* ensure capacity up to max(n,16) because we need 16 seed values */
    size_t needed = (size_t)n + 1;
    size_t seed_needed = 17; /* indices 0..16 */
    if (needed < seed_needed) needed = seed_needed;
    if (capacity < needed) {
        size_t new_cap = capacity ? capacity : 16;
        while (new_cap < needed) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return 0;      /* allocation failed, return 0 */
        cache = tmp;
        capacity = new_cap;
    }
 
    /* initialize seed values only once */
    if (!initialized) {
        /* indices 0..16, index 0 unused but kept zero */
        cache[0] = (seq_t)0;
        cache[1] = (seq_t)0;
        cache[2] = (seq_t)30;
        cache[3] = (seq_t)440;
        cache[4] = (seq_t)2898;
        cache[5] = (seq_t)11756;
        cache[6] = (seq_t)36864;
        cache[7] = (seq_t)95832;
        cache[8] = (seq_t)219092;
        cache[9] = (seq_t)452368;
        cache[10] = (seq_t)864810;
        cache[11] = (seq_t)1551080;
        cache[12] = (seq_t)2642774;
        cache[13] = (seq_t)4310012;
        cache[14] = (seq_t)6776612;
        cache[15] = (seq_t)10320160;
        cache[16] = (seq_t)15292160;
        max_computed = 16;
        initialized = true;
    }
 
    /* if requested n already computed, return it */
    if (n <= max_computed) return cache[n];
 
    /* compute missing values up to n */
    for (idx_t k = max_computed + 1; k <= n; ++k) {
        /* recurrence requires terms k-1 .. k-16 */
        seq_t val = (seq_t)0;
        /* positive terms */
        val += (seq_t)2 * cache[k-1];
        val +=           cache[k-2];
        val += (seq_t)5 * cache[k-6];
        val += (seq_t)2 * cache[k-7];
        val += (seq_t)2 * cache[k-11];
        val += (seq_t)2 * cache[k-12];
        val += (seq_t)2 * cache[k-13];
        val +=           cache[k-16];
        /* negative terms */
        val -= (seq_t)2 * cache[k-3];
        val -= (seq_t)2 * cache[k-4];
        val -= (seq_t)2 * cache[k-5];
        val -= (seq_t)2 * cache[k-9];
        val -= (seq_t)5 * cache[k-10];
        val -=           cache[k-14];
        val -= (seq_t)2 * cache[k-15];
        cache[k] = val;
    }
    max_computed = n;
    return cache[n];
}
