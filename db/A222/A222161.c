/*
 * A222161  Number of (n+3)X1 arrays of occupancy after each element moves up
 *          to +-3 places but not 0 and without 2-loops.
 * https://oeis.org/A222161
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 7*a(n-1) -13*a(n-2) -3*a(n-3) +21*a(n-4) -41*a(n-6)
 *            +83*a(n-7) +30*a(n-8) -216*a(n-9) +17*a(n-10) +214*a(n-11)
 *            -19*a(n-12) -101*a(n-13) +5*a(n-14) -9*a(n-15) -5*a(n-16)
 *            +45*a(n-17) -12*a(n-18) -21*a(n-19) +2*a(n-20) +a(n-21); Initial
 *            values for n = 1 … 21 are given in the OEIS entry
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 16, u64 n <= 34, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    /* static cache, grows as needed */
    static seq_t *cache = NULL;
    static idx_t cache_cap = 0;   /* current allocated size (max index + 1) */

    /* ensure we have room for index n */
    if (n + 1 > cache_cap) {
        idx_t new_cap = n + 1;
        seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!new_cache) abort();         /* out of memory */
        /* initialise new slots to zero (default 0) */
        for (idx_t i = cache_cap; i < new_cap; ++i) new_cache[i] = 0;
        cache = new_cache;
        cache_cap = new_cap;
    }

    /* seed values for n = 1 … 21 */
    static const seq_t seeds[21] = {
        (seq_t)13ULL,      (seq_t)57ULL,      (seq_t)205ULL,
        (seq_t)733ULL,     (seq_t)2627ULL,    (seq_t)9305ULL,
        (seq_t)32576ULL,   (seq_t)113448ULL,  (seq_t)394406ULL,
        (seq_t)1369551ULL, (seq_t)4750768ULL, (seq_t)16468347ULL,
        (seq_t)57064823ULL,(seq_t)197691644ULL,(seq_t)684764755ULL,
        (seq_t)2371639753ULL,(seq_t)8213463074ULL,(seq_t)28443621204ULL,
        (seq_t)98498816070ULL,(seq_t)341089856906ULL,(seq_t)1181139073972ULL
    };

    /* compute all values up to n in increasing order */
    for (idx_t i = 1; i <= n; ++i) {
        /* skip already computed values */
        if (cache[i] != 0) continue;

        /* use seed if within range */
        if (i <= 21) {
            cache[i] = seeds[i - 1];
            continue;
        }

        /* recurrence for i >= 22 */
        seq_t sum = 0;
        sum += (seq_t)7  * cache[i - 1];
        sum -= (seq_t)13 * cache[i - 2];
        sum -= (seq_t)3  * cache[i - 3];
        sum += (seq_t)21 * cache[i - 4];
        sum -= (seq_t)41 * cache[i - 6];
        sum += (seq_t)83 * cache[i - 7];
        sum += (seq_t)30 * cache[i - 8];
        sum -= (seq_t)216* cache[i - 9];
        sum += (seq_t)17 * cache[i -10];
        sum += (seq_t)214* cache[i -11];
        sum -= (seq_t)19 * cache[i -12];
        sum -= (seq_t)101* cache[i -13];
        sum += (seq_t)5  * cache[i -14];
        sum -= (seq_t)9  * cache[i -15];
        sum -= (seq_t)5  * cache[i -16];
        sum += (seq_t)45 * cache[i -17];
        sum -= (seq_t)12 * cache[i -18];
        sum -= (seq_t)21 * cache[i -19];
        sum += (seq_t)2  * cache[i -20];
        sum +=          cache[i -21];

        cache[i] = sum;
    }

    return cache[n];
}
