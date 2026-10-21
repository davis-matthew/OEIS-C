/*
 * A205754  Number of 3X(n+1) 0..2 arrays with every 2X2 subblock having the
 *          same number of equal edges as its horizontal neighbors and a
 *          different number from its vertical neighbors, and new values 0..2
 *          introduced in row major order.
 * https://oeis.org/A205754
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 5, u32 n <= 13, u64 n <= 29, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = 8*a(n-1) - 2*a(n-2) - 98*a(n-3) + 73*a(n-4)
 *               + 496*a(n-5) - 196*a(n-6) - 1120*a(n-7) - 54*a(n-8)
 *               + 813*a(n-9) + 171*a(n-10) - 90*a(n-11),
 *   with a(1)=74, a(2)=284, a(3)=1074, a(4)=4256, a(5)=16986,
 *   a(6)=68692, a(7)=279504, a(8)=1143842, a(9)=4699050,
 *   a(10)=19364230, a(11)=79981548. */

#include <stdlib.h>

static seq_t *cache = NULL;          /* cached values, 1‑based indexing */
static size_t    cache_size = 0;     /* current capacity of cache   */
static idx_t     last_computed = 0;  /* largest n for which cache[n] is valid */

/* Ensure cache is large enough for indices up to n and that all required
 * values have been computed. */
static void ensure_cache(idx_t n)
{
    /* 1. Allocate (or grow) the cache so it can hold indices 0..n. */
    if (cache == NULL) {
        cache_size = (n + 1 > 12 ? n + 1 : 12);        /* at least 12 slots */
        cache = (seq_t *)malloc(cache_size * sizeof(seq_t));
        if (!cache) return;                           /* allocation failed */

        /* 2. Initialize known initial values (index 0 is unused). */
        cache[0] = (seq_t)0;
        cache[1] = (seq_t)74;
        cache[2] = (seq_t)284;
        cache[3] = (seq_t)1074;
        cache[4] = (seq_t)4256;
        cache[5] = (seq_t)16986;
        cache[6] = (seq_t)68692;
        cache[7] = (seq_t)279504;
        cache[8] = (seq_t)1143842;
        cache[9] = (seq_t)4699050;
        cache[10] = (seq_t)19364230;
        cache[11] = (seq_t)79981548;
        last_computed = 11;          /* we have values up to index 11 */
    } else if ((size_t)n + 1 > cache_size) {
        size_t new_size = cache_size * 2;
        if ((size_t)n + 1 > new_size) new_size = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp) return;           /* allocation failed, keep old cache */
        cache = tmp;
        cache_size = new_size;
    }

    /* 3. Compute any missing terms using the recurrence. */
    if (n > last_computed) {
        /* Coefficients with signs included. */
        static const int coef[12] = {
            0,          /* unused, 0-index */
            8,          /* a(n-1) */
            -2,         /* a(n-2) */
            -98,        /* a(n-3) */
            73,         /* a(n-4) */
            496,        /* a(n-5) */
            -196,       /* a(n-6) */
            -1120,      /* a(n-7) */
            -54,        /* a(n-8) */
            813,        /* a(n-9) */
            171,        /* a(n-10) */
            -90         /* a(n-11) */
        };

        for (idx_t i = last_computed + 1; i <= n; ++i) {
            seq_t sum = (seq_t)0;
            for (int k = 1; k <= 11; ++k) {
                sum += (seq_t)coef[k] * cache[i - k];
            }
            cache[i] = sum;
        }
        last_computed = n;
    }
}

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;   /* undefined for n < 1 */
    ensure_cache(n);
    return cache[n];
}
