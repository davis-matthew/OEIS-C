/*
 * A124950  Number of base 22 circular n-digit numbers with adjacent digits
 *          differing by 2 or less.
 * https://oeis.org/A124950
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(0) = 1. For n ≥ 1, a(n) is the trace of the n‑th power of the
 *            22×22 adjacency matrix A where A[i][j] = 1 iff |i−j| ≤ 2
 *            (otherwise 0)
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 13, u64 n <= 27, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation stores all previously computed values in a
   dynamically grown array.  The array is resized only when a larger n
   is requested.  For C++ builds the array is allocated with new[]/delete[]
   so that non‑trivial seq_t types (arbitrary‑precision integers) are
   copied correctly.  For C builds, the array is managed with malloc/realloc. */

#include <stdlib.h>
#ifdef __cplusplus
/* C++ builds need <cstddef> for size_t */
#include <cstddef>
#endif

/* seq_t and idx_t are supplied by the build environment */
seq_t a(idx_t n)
{
    /* ---------- cache of already computed values ---------- */
    static seq_t *cache = NULL;     /* array of a(k) for 0 ≤ k < cache_cap */
    static size_t cache_cap = 0;    /* allocated size (number of elements) */
    static size_t cache_len = 0;    /* number of values actually stored */

    /* Allocate & initialise cache on first use */
    if (cache_cap == 0) {
        cache_cap = 16;                   /* initial capacity */
        cache_len  = 1;
#ifdef __cplusplus
        cache = new seq_t[cache_cap];
#else
        cache = (seq_t*)malloc(cache_cap * sizeof(seq_t));
#endif
        if (!cache) abort();
        cache[0] = (seq_t)1;              /* a(0) = 1 */
    }

    /* Grow cache if request exceeds current capacity */
    if ((size_t)n + 1 > cache_cap) {
        size_t new_cap = cache_cap;
        while (new_cap <= (size_t)n) new_cap <<= 1;
        seq_t *new_cache;
#ifdef __cplusplus
        new_cache = new seq_t[new_cap];
#else
        new_cache = (seq_t*)malloc(new_cap * sizeof(seq_t));
#endif
        if (!new_cache) abort();
        /* copy previously computed values */
        for (size_t i = 0; i < cache_len; ++i) new_cache[i] = cache[i];
#ifdef __cplusplus
        delete[] cache;
#else
        free(cache);
#endif
        cache = new_cache;
        cache_cap = new_cap;
    }

    /* Return value immediately if already computed */
    if ((size_t)n < cache_len) {
        return cache[n];
    }

    /* ---------- matrix power computation ---------- */
    static seq_t M[22][22];   /* current power of A  (M = A^k) */
    static idx_t cur_pow = 0; /* exponent currently stored in M (0 means A^0 not stored) */

    /* Compute until cur_pow == n */
    while (cur_pow < n) {
        if (cur_pow == 0) {
            /* initialise M as A (first power) */
            for (int i = 0; i < 22; ++i) {
                for (int j = 0; j < 22; ++j) {
                    M[i][j] = ((i >= j - 2) && (i <= j + 2)) ? (seq_t)1 : (seq_t)0;
                }
            }
            /* trace corresponds to a(1) */
        } else {
            /* multiply current M by A (banded) to obtain A^(cur_pow+1) */
            seq_t tmp[22][22];
            for (int i = 0; i < 22; ++i) {
                for (int j = 0; j < 22; ++j) {
                    tmp[i][j] = (seq_t)0;
                }
            }
            for (int i = 0; i < 22; ++i) {
                for (int k = 0; k < 22; ++k) {
                    seq_t val = M[i][k];
                    if (val == (seq_t)0) continue;
                    int low  = k - 2; if (low < 0)      low = 0;
                    int high = k + 2; if (high > 21)    high = 21;
                    for (int j = low; j <= high; ++j) {
                        tmp[i][j] += val;
                    }
                }
            }
            /* copy result back into M */
            for (int i = 0; i < 22; ++i) {
                for (int j = 0; j < 22; ++j) {
                    M[i][j] = tmp[i][j];
                }
            }
        }

        /* trace of the current matrix is a(cur_pow+1) */
        seq_t trace = (seq_t)0;
        for (int i = 0; i < 22; ++i) {
            trace += M[i][i];
        }

        /* advance the exponent */
        ++cur_pow;      /* now cur_pow equals exponent for which M was computed */

        /* store the result in the cache if space permits */
        if (cur_pow < cache_cap) {
            cache[cur_pow] = trace;
        }

        /* update logical size of cache */
        if ((size_t)cur_pow + 1 > cache_len) {
            cache_len = (size_t)cur_pow + 1;
        }
    }

    return cache[n];
}
