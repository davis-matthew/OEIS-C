/*
 * A219059  Hilltop maps: number of nX4 binary arrays indicating the locations
 *          of corresponding elements not exceeded by any horizontal, diagonal
 *          or antidiagonal neighbor in a random 0..2 nX4 array.
 * https://oeis.org/A219059
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   empirical linear recurrence for A219059 a(n) = 14*a(n-1)
 *            +28*a(n-2) +55*a(n-3) +106*a(n-4) +358*a(n-5) -50*a(n-6)
 *            -102*a(n-7) -38*a(n-8) -122*a(n-9) -210*a(n-10) +18*a(n-11)
 *            +27*a(n-12) +18*a(n-14) +27*a(n-15) with initial terms: a(1)=13,
 *            a(2)=249, a(3)=4049, a(4)=64577, a(5)=1032785, a(6)=165205...
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 16, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* static cache of computed values */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;          /* amount of allocated slots */

    /* initialise cache with known initial values (indices 1..15) */
    if (cache == NULL) {
        size_t init_cap = 16;             /* we allow indices up to 16 for safety */
        cache = (seq_t *)malloc((init_cap + 1) * sizeof(seq_t));
        cache[0] = (seq_t)0;
        cache[1] = (seq_t)13ULL;
        cache[2] = (seq_t)249ULL;
        cache[3] = (seq_t)4049ULL;
        cache[4] = (seq_t)64577ULL;
        cache[5] = (seq_t)1032785ULL;
        cache[6] = (seq_t)16520553ULL;
        cache[7] = (seq_t)264274797ULL;
        cache[8] = (seq_t)4227506465ULL;
        cache[9] = (seq_t)67625780417ULL;
        cache[10] = (seq_t)1081783483265ULL;
        cache[11] = (seq_t)17304872546433ULL;
        cache[12] = (seq_t)276819362795425ULL;
        cache[13] = (seq_t)4428172435248429ULL;
        cache[14] = (seq_t)70835764221804249ULL;
        cache[15] = (seq_t)1133132362454651729ULL;
        /* zero initialise remaining allocated slots */
        for (size_t i = 16; i <= init_cap; ++i) cache[i] = (seq_t)0;
        cache_cap = init_cap + 1;
    }

    /* ensure the cache is large enough for index n */
    if (n + 1 > cache_cap) {
        size_t new_cap = (size_t)((n + 1) * 2);
        cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        for (size_t i = cache_cap; i < new_cap; ++i) cache[i] = (seq_t)0;
        cache_cap = new_cap;
    }

    /* compute missing values up to n */
    for (idx_t i = 1; i <= n; ++i) {
        if (i <= 15 || cache[i] != (seq_t)0) continue; /* already known */

        seq_t val = 0;
        val += 14 * cache[i-1];
        val += 28 * cache[i-2];
        val += 55 * cache[i-3];
        val += 106 * cache[i-4];
        val += 358 * cache[i-5];
        val -= 50 * cache[i-6];
        val -= 102 * cache[i-7];
        val -= 38 * cache[i-8];
        val -= 122 * cache[i-9];
        val -= 210 * cache[i-10];
        val += 18 * cache[i-11];
        val += 27 * cache[i-12];
        val += 18 * cache[i-14];
        val += 27 * cache[i-15];
        cache[i] = val;
    }

    return cache[n];
}
