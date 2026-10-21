/*
 * A202967  Number of arrays of 7 integers in -n..n with sum zero and adjacent
 *          elements differing in absolute value.
 * https://oeis.org/A202967
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) follows the recurrence a(n) = 2*a(n-1) -a(n-2) +a(n-3)
 *            -2*a(n-5) +a(n-6) -3*a(n-7) +3*a(n-8) -a(n-9) +2*a(n-10)
 *            +a(n-11) -a(n-13) -2*a(n-14) +a(n-15) -3*a(n-16) +3*a(n-17)
 *            -a(n-18) +2*a(n-19) -a(n-21) +a(n-22) -2*a(n-23) +a(n-24) with
 *            initial values a(0)=0, a(1)=6, a(2)=524, a(3)=9872, a(4)=...
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 23, u64 n <= 210, u128 n <= 210,
 *            big any n
 * Helpers:   mul_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

/* Helper to multiply a seq_t by a small integer.
   The implementation is in ../utils/mul_small.h. */
#include "../utils/mul_small.h"

seq_t a(idx_t n)
{
    /* static cache of computed values */
    static seq_t *cache      = NULL;
    static idx_t  cache_size = 0;
    static bool    init_done  = false;

    if (!init_done) {
        /* allocate space for indices 0..24 (25 elements) */
        cache_size = 25;
        cache = (seq_t *)calloc(cache_size, sizeof(seq_t));
        if (!cache) return 0;   /* allocation failed */

        /* set the known initial values */
        cache[0] = 0;
        cache[1] = 6;
        cache[2] = 524;
        cache[3] = 9872;
        cache[4] = 66208;
        cache[5] = 289536;
        cache[6] = 941086;
        cache[7] = 2537024;
        cache[8] = 5933492;
        cache[9] = 12515772;
        cache[10] = 24295760;
        cache[11] = 44188086;
        cache[12] = 76110890;
        cache[13] = 125357152;
        cache[14] = 198686606;
        cache[15] = 304811136;
        cache[16] = 454458544;
        cache[17] = 660981870;
        cache[18] = 940393644;
        cache[19] = 1312104510;
        cache[20] = 1798910920;
        cache[21] = 2427885688;
        cache[22] = 3230300850;
        cache[23] = 4242680088;
        cache[24] = 0; /* placeholder, will be computed */

        init_done = true;
    }

    /* if n is within cached range, return directly */
    if (n < cache_size) {
        /* for n==24, compute it only once */
        if (cache[n] == 0 && n == 24) {
            seq_t val = 0;
            val += mul_small(cache[23], 2);
            val -= cache[22];
            val += cache[21];
            val -= mul_small(cache[19], 2);
            val += cache[18];
            val -= mul_small(cache[17], 3);
            val += mul_small(cache[16], 3);
            val -= cache[15];
            val += mul_small(cache[14], 2);
            val += cache[13];
            val -= cache[11];
            val -= mul_small(cache[10], 2);
            val += cache[9];
            val -= mul_small(cache[8], 3);
            val += mul_small(cache[7], 3);
            val -= cache[6];
            val += mul_small(cache[5], 2);
            val -= cache[3];
            val += cache[2];
            val -= mul_small(cache[1], 2);
            /* cache[0] has no effect */
            cache[24] = val;
        }
        return cache[n];
    }

    /* extend cache up to n */
    idx_t new_size = n + 1;
    seq_t *new_cache = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
    if (!new_cache) return 0; /* realloc failed */
    cache = new_cache;
    /* initialise new elements to zero (constructors may be invoked) */
    for (idx_t i = cache_size; i < new_size; ++i)
        cache[i] = 0;
    cache_size = new_size;

    /* compute missing terms */
    for (idx_t i = 24; i <= n; ++i) {
        if (cache[i] != 0) continue;   /* already computed */
        seq_t val = 0;
        val += mul_small(cache[i-1], 2);
        val -= cache[i-2];
        val += cache[i-3];
        val -= mul_small(cache[i-5], 2);
        val += cache[i-6];
        val -= mul_small(cache[i-7], 3);
        val += mul_small(cache[i-8], 3);
        val -= cache[i-9];
        val += mul_small(cache[i-10], 2);
        val += cache[i-11];
        val -= cache[i-13];
        val -= mul_small(cache[i-14], 2);
        val += cache[i-15];
        val -= mul_small(cache[i-16], 3);
        val += mul_small(cache[i-17], 3);
        val -= cache[i-18];
        val += mul_small(cache[i-19], 2);
        val -= cache[i-21];
        val += cache[i-22];
        val -= mul_small(cache[i-23], 2);
        val += cache[i-24];
        cache[i] = val;
    }

    return cache[n];
}
