/*
 * A028964  Numbers represented by quadratic form with Gram matrix [ 2, 1, 1;
 *          1, 4, 2; 1, 2, 4 ], divided by 2.
 * https://oeis.org/A028964
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = numbers represented by quadratic form with Gram matrix [
 *            2, 1, 1; 1, 4, 2; 1, 2, 4 ], divided by 2
 * Range:     u8 n <= 10, u16 n <= 72, u32 n <= 72, u64 n <= 72, u128 n <= 72,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Static cache of sequence values */
static seq_t *cache = NULL;      /* sorted, unique values */
static size_t cache_cap = 0;     /* allocated size of cache */
static size_t cache_cnt = 0;     /* number of terms stored */
static int last_B = 0;           /* last bound used in building cache */

/* Compare two seq_t values for qsort */
static int cmp_seq(const void *a, const void *b)
{
    const seq_t *x = (const seq_t *)a;
    const seq_t *y = (const seq_t *)b;
    if (*x < *y) return -1;
    if (*x > *y) return 1;
    return 0;
}

/* Build the entire cache for all integer triples with coordinates
   from -B to B.  The resulting cache will contain all values
   v = x^2 + 2y^2 + 2z^2 + xy + xz + 2yz with v <= (2B+1)^3 * something. */
static void recalc_cache(int B)
{
    if (B <= 0) B = 1;
    size_t dim = (size_t)(2 * B + 1);
    size_t N = dim * dim * dim;        /* number of triples */
    seq_t *tmp = (seq_t *)malloc(N * sizeof(seq_t));
    if (!tmp) {
        /* memory allocation failed; leave cache unchanged */
        return;
    }

    size_t idx = 0;
    for (int x = -B; x <= B; ++x) {
        long long vx = (long long)x;
        for (int y = -B; y <= B; ++y) {
            long long vy = (long long)y;
            for (int z = -B; z <= B; ++z) {
                long long vz = (long long)z;
                long long v = vx * vx
                            + 2 * vy * vy
                            + 2 * vz * vz
                            + vx * vy
                            + vx * vz
                            + 2 * vy * vz;
                tmp[idx++] = (seq_t)v;
            }
        }
    }

    qsort(tmp, N, sizeof(seq_t), cmp_seq);

    /* Free previous cache if any */
    if (cache) free(cache);
    cache = NULL;
    cache_cap = 0;
    cache_cnt = 0;

    /* Copy unique values into new cache */
    for (size_t i = 0; i < N; ++i) {
        if (i == 0 || tmp[i] != tmp[i - 1]) {
            if (cache_cnt == cache_cap) {
                size_t new_cap = cache_cap ? cache_cap * 2 : 4096;
                seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!new_cache) {
                    /* realloc failed; clean up and exit */
                    free(tmp);
                    if (cache) free(cache);
                    cache = NULL;
                    cache_cap = 0;
                    cache_cnt = 0;
                    return;
                }
                cache = new_cache;
                cache_cap = new_cap;
            }
            cache[cache_cnt++] = tmp[i];
        }
    }

    free(tmp);
    last_B = B;
}

/* Ensure the cache contains at least n terms; grow if necessary. */
static void ensure_cache(idx_t n)
{
    if (cache == NULL) {
        /* Initial build with a moderate bound. */
        recalc_cache(80);
    }
    while ((idx_t)cache_cnt < n) {
        int newB = last_B > 0 ? last_B * 2 : 80;
        recalc_cache(newB);
    }
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return 0;         /* undefined offset, return 0 safety */
    ensure_cache(n);
    return cache[n - 1];
}
