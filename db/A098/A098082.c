/*
 * A098082  Absolute values of elements of flattened successive matrices B(k),
 *          k>=0, read by rows where B(k) = M^k*C where M=[[-1, 1, 0], [0, -1,
 *          1], [1, 1, -1]] and C = [[0, 1, 1], [1, 1, 1], [1, 1, 2]].
 * https://oeis.org/A098082
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, less
 * Formula:   not stated
 * Range:     u8 n <= 90, u16 n <= 90, u32 n <= 90, u64 n <= 90, u128 n <= 90,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A098082 – absolute values of the flattened successive matrices
    B(k) = M^k * C,
    where M = [[-1,1,0],[0,-1,1],[1,1,-1]] and
    C = [[0,1,1],[1,1,1],[1,1,2]].
    The sequence is flattened row‐wise: a(n) = |B(⌊n/9⌋)[n mod 9]|. */

#include <stdlib.h>
#include <stdint.h>

#define MAT_SZ 9                     /* 3×3 matrix entries */

typedef int64_t mat_entry_t;        /* signed intermediate type */

/* Row‑wise flattened initial matrix C. */
static const mat_entry_t C[MAT_SZ] = {
    0, 1, 1,
    1, 1, 1,
    1, 1, 2
};

/* Cached matrices: a linear buffer storing matrices for k = 0 … cache_kmax. */
static mat_entry_t *mat_cache  = NULL;  /* pointer to buffer */
static size_t       cache_kmax = 0;      /* largest k currently computed */
static size_t       cache_cap  = 0;      /* capacity in number of matrices */

/* Ensure the cache contains matrices up to index k (inclusive). */
static void ensure_cache(size_t k)
{
    if (mat_cache == NULL) {                  /* first allocation */
        cache_cap = (k + 1 > 4) ? (k + 1) : 4;
        mat_cache = (mat_entry_t *)malloc(cache_cap * MAT_SZ * sizeof(mat_entry_t));
        for (size_t cur = 0; cur <= k; ++cur) {
            mat_entry_t *curm = &mat_cache[cur * MAT_SZ];
            if (cur == 0) {                  /* B(0) = C */
                for (int i = 0; i < MAT_SZ; ++i) curm[i] = C[i];
            } else {
                mat_entry_t *prev = &mat_cache[(cur - 1) * MAT_SZ];
                /* B(cur) = M * B(cur-1) */
                curm[0] = -prev[0] + prev[3];
                curm[1] = -prev[1] + prev[4];
                curm[2] = -prev[2] + prev[5];
                curm[3] = -prev[3] + prev[6];
                curm[4] = -prev[4] + prev[7];
                curm[5] = -prev[5] + prev[8];
                curm[6] =  prev[0] + prev[3] - prev[6];
                curm[7] =  prev[1] + prev[4] - prev[7];
                curm[8] =  prev[2] + prev[5] - prev[8];
            }
        }
        cache_kmax = k;
        return;
    }

    if (k <= cache_kmax) return;            /* already available */

    /* Grow buffer to at least k+1 matrices. */
    size_t new_cap = cache_cap;
    if (new_cap == 0) new_cap = 4;
    while (new_cap <= k) new_cap <<= 1;
    mat_entry_t *tmp = (mat_entry_t *)realloc(mat_cache,
                              new_cap * MAT_SZ * sizeof(mat_entry_t));
    mat_cache = tmp;
    cache_cap  = new_cap;

    /* Compute new matrices from cache_kmax+1 up to k. */
    for (size_t cur = cache_kmax + 1; cur <= k; ++cur) {
        mat_entry_t *prev = &mat_cache[(cur - 1) * MAT_SZ];
        mat_entry_t *curm = &mat_cache[cur * MAT_SZ];
        curm[0] = -prev[0] + prev[3];
        curm[1] = -prev[1] + prev[4];
        curm[2] = -prev[2] + prev[5];
        curm[3] = -prev[3] + prev[6];
        curm[4] = -prev[4] + prev[7];
        curm[5] = -prev[5] + prev[8];
        curm[6] =  prev[0] + prev[3] - prev[6];
        curm[7] =  prev[1] + prev[4] - prev[7];
        curm[8] =  prev[2] + prev[5] - prev[8];
    }
    cache_kmax = k;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    size_t k   = (size_t)(n / 9);
    size_t pos = (size_t)(n % 9);
    ensure_cache(k);
    mat_entry_t val = mat_cache[k * MAT_SZ + pos];
    return (val >= 0) ? (seq_t)val : (seq_t)(-val);
}
