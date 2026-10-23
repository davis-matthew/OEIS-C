/*
 * A147723  Riordan array (1, x(1 - 4x)/(1 - 7x + 3x^2)).
 * https://oeis.org/A147723
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 15, u16 n <= 36, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A147723 – Riordan array (1, x(1-4x)/(1-7x+3x^2))
   T(n,k) = 7*T(n-1,k) + T(n-1,k-1) - 3*T(n-2,k) - 4*T(n-2,k-1)
   with T(0,0)=1, T(1,0)=0, T(1,1)=1, T(2,0)=0, T(2,1)=3, T(2,2)=1,
   T(n,k)=0 if k<0 or k>n.  The array a(n) is the entry T(k,m) where
   k is the largest integer with k*(k+1)/2 ≤ n and m = n - k*(k+1)/2.  */

#include <stdint.h>
#include <stdlib.h>

/* Integer square‑root for unsigned 64‑bit values (no math.h). */
static uint64_t u64_isqrt(uint64_t v)
{
    uint64_t r = 0;
    uint64_t bit = 1ULL << 62;          /* largest power of four ≤ v */
    while (bit != 0) {
        if (v >= r + bit) {
            v -= r + bit;
            r = (r >> 1) + bit;
        } else {
            r >>= 1;
        }
        bit >>= 2;
    }
    return r;
}

/* Sequence retrieval function. */
seq_t a(idx_t n)
{
    /* Handle the very first element separately. */
    if (n == 0U) {
        return (seq_t)1;
    }

    /* Compute k = floor((sqrt(8*n+1)-1)/2). */
    uint64_t t = 8ULL * (uint64_t)n + 1ULL;
    uint64_t s = u64_isqrt(t);
    uint64_t k64 = (s - 1ULL) / 2ULL;
    size_t k = (size_t)k64;                     /* row index */

    /* Triangular number T_k = k*(k+1)/2 in 64‑bit. */
    uint64_t Tk = (uint64_t)k * (k + 1ULL) / 2ULL;

    /* Column index m = n - T_k (fits in idx_t). */
    idx_t m_idx = (idx_t)n - Tk;
    size_t m = (size_t)m_idx;                    /* 0 ≤ m ≤ k */

    /* Static cache of computed rows. */
    static seq_t **tri = NULL;
    static size_t tri_rows = 0;   /* number of rows currently stored */

    /* Grow the cache if necessary. */
    if (k >= tri_rows) {
        size_t new_rows = k + 1;
        seq_t **new_tri = (seq_t **)realloc(tri, new_rows * sizeof(seq_t *));
        tri = new_tri;
        for (size_t i = tri_rows; i < new_rows; ++i) {
            tri[i] = (seq_t *)calloc(i + 1, sizeof(seq_t));
            /* Fill the new row i. */
            if (i == 0) {
                tri[0][0] = (seq_t)1;
                continue;
            }
            tri[i][0] = (seq_t)0;         /* m = 0 for i > 0 */
            if (i == 1) {                 /* base row */
                tri[1][1] = (seq_t)1;
                continue;
            }
            /* Rows i ≥ 2 */
            for (size_t j = 1; j < i; ++j) {
                seq_t val = (seq_t)7 * tri[i - 1][j];
                val += tri[i - 1][j - 1];
                if (j <= i - 2) {
                    val -= (seq_t)3 * tri[i - 2][j];
                }
                if (j - 1 <= i - 2) {
                    val -= (seq_t)4 * tri[i - 2][j - 1];
                }
                tri[i][j] = val;
            }
            tri[i][i] = (seq_t)1;         /* diagonal */
        }
        tri_rows = new_rows;
    }

    return tri[k][m];
}
