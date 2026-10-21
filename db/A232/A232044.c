/*
 * A232044  Number of nX5 0..1 arrays with no element less than a strict
 *          majority of its horizontal and antidiagonal neighbors.
 * https://oeis.org/A232044
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 16, u128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A232044: Number of n×5 0..1 arrays with no element less than a strict majority of its horizontal and antidiagonal neighbors.
   Recurrence (for n > 15):
      a(n) = 12*a(n-1) + 99*a(n-2) - 133*a(n-3) - 1593*a(n-4)
           + 638*a(n-5) + 9006*a(n-6) - 2223*a(n-7) - 18168*a(n-8)
           + 4203*a(n-9) + 6433*a(n-10) - 453*a(n-11) - 1184*a(n-12)
           + 28*a(n-13) + 96*a(n-14)
   Initial values:
      a(1)  = 12
      a(2)  = 79
      a(3)  = 1315
      a(4)  = 22063
      a(5)  = 377676
      a(6)  = 6430408
      a(7)  = 109609484
      a(8)  = 1868028342
      a(9)  = 31836538191
      a(10) = 542586883485
      a(11) = 9247235554661
      a(12) = 157599508115116
      a(13) = 2685948646284826
      a(14) = 45776288238345925
      a(15) = 780159569752010830
*/

#include <stdlib.h>
#include <stdint.h>

/* Static cache for computed values: cache[i] holds a(i+1). */
static seq_t *cache = NULL;
static idx_t  cache_len = 0;

/* Pre‑computed initial terms (index 0 → a(1)). */
static const seq_t init_vals[15] = {
    (seq_t)12ULL,
    (seq_t)79ULL,
    (seq_t)1315ULL,
    (seq_t)22063ULL,
    (seq_t)377676ULL,
    (seq_t)6430408ULL,
    (seq_t)109609484ULL,
    (seq_t)1868028342ULL,
    (seq_t)31836538191ULL,
    (seq_t)542586883485ULL,
    (seq_t)9247235554661ULL,
    (seq_t)157599508115116ULL,
    (seq_t)2685948646284826ULL,
    (seq_t)45776288238345925ULL,
    (seq_t)780159569752010830ULL
};

/* Compute a(n), n ≥ 1 (1‑based). */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* undefined, return zero */

    /* Ensure cache is large enough. */
    if (cache_len < n) {
        size_t new_len = (size_t)n;
        seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        if (!tmp) { free(cache); cache = NULL; cache_len = 0; return (seq_t)0; }
        cache = tmp;

        /* Fill initial values up to 15 if they were not already set. */
        for (idx_t i = cache_len; i < new_len && i < 15; ++i)
            cache[i] = init_vals[i];

        /* Compute remaining terms using the recurrence. */
        for (idx_t i = cache_len; i < new_len; ++i) {
            /* i >= 15 (zero‑based) corresponds to n = i+1 >= 16. */
            if (i < 15) continue;  /* already initialized */

            seq_t val = 0;
            val += cache[i-1]  * (seq_t)12ULL;
            val += cache[i-2]  * (seq_t)99ULL;
            val -= cache[i-3]  * (seq_t)133ULL;
            val -= cache[i-4]  * (seq_t)1593ULL;
            val += cache[i-5]  * (seq_t)638ULL;
            val += cache[i-6]  * (seq_t)9006ULL;
            val -= cache[i-7]  * (seq_t)2223ULL;
            val -= cache[i-8]  * (seq_t)18168ULL;
            val += cache[i-9]  * (seq_t)4203ULL;
            val += cache[i-10] * (seq_t)6433ULL;
            val -= cache[i-11] * (seq_t)453ULL;
            val -= cache[i-12] * (seq_t)1184ULL;
            val += cache[i-13] * (seq_t)28ULL;
            val += cache[i-14] * (seq_t)96ULL;

            cache[i] = val;
        }

        cache_len = new_len;
    }

    return cache[n-1];
}
