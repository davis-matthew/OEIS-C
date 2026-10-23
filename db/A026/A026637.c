/*
 * A026637  Triangular array T read by rows: T(n,0) = T(n,n) = 1 for n >= 0,
 *          T(n,1) = T(n,n-1) = floor((3*n-1)/2) for n >= 1, otherwise T(n,k)
 *          = T(n-1,k-1) + T(n-1,k) for 2 <= k <= n-2, n >= 4.
 * https://oeis.org/A026637
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   not stated
 * Range:     u8 n <= 58, u16 n <= 196, u32 n <= 644, u64 n <= 2310,
 *            u128 n <= 5150, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A026637: Triangular array T read by rows.
   Definition (OEIS):
     T(n,0) = T(n,n) = 1
     T(n,1) = T(n,n-1) = floor((3*n-1)/2)      for n ≥ 1
     T(n,k) = T(n-1,k-1) + T(n-1,k)             for 2 ≤ k ≤ n-2 , n ≥ 4
   a(n) is the n‑th term of the triangle read row by row
   (rows start at n = 0).                                         */

#include <stdlib.h>

static inline idx_t isqrt_u64(idx_t x)
{
    idx_t lo = 0, hi = ((idx_t)1 << 32);   /* hi^2 ≥ x for all 0 ≤ x < 2^64 */
    while (lo < hi) {
        idx_t mid = (lo + hi + 1) >> 1;
        if (((__int128)mid * mid) <= x)
            lo = mid;
        else
            hi = mid - 1;
    }
    return lo;
}

/* row of the n-th term (0‑based) */
static inline idx_t row_of(idx_t n)
{
    /* r = floor((sqrt(8*n+1)-1)/2) */
    return ( isqrt_u64(8*n + 1) - 1 ) / 2;
}

/* starting index of row r */
static inline idx_t row_start(idx_t r)
{
    return ( (__int128)r * (r + 1) ) / 2;
}

/* cache of computed rows (pointer to arrays of seq_t) */
static seq_t **cache_rows = NULL;
static size_t  cached_cnt = 0;          /* number of rows stored so far */

seq_t a(idx_t n)
{
    idx_t r = row_of(n);
    idx_t k = n - row_start(r);

    size_t target_row = (size_t)r;

    /* Expand cache if needed */
    if (target_row >= cached_cnt) {
        size_t new_cnt = target_row + 1;
        seq_t **new_cache = (seq_t**)realloc(cache_rows,
                                            new_cnt * sizeof(seq_t*));
        if (!new_cache) return (seq_t)0;          /* allocation failure */
        cache_rows = new_cache;

        for (size_t rr = cached_cnt; rr <= target_row; ++rr) {
            size_t len = rr + 1;
            seq_t *row = (seq_t*)malloc(len * sizeof(seq_t));
            if (!row) return (seq_t)0;           /* allocation failure */

            row[0] = (seq_t)1;
            row[rr] = (seq_t)1;

            if (rr >= 1) {
                seq_t v = ((seq_t)3 * rr - (seq_t)1) / (seq_t)2;
                row[1] = v;
                row[rr-1] = v;
            }

            if (rr >= 4) {
                for (size_t kk = 2; kk <= rr-2; ++kk) {
                    row[kk] = cache_rows[rr-1][kk-1] + cache_rows[rr-1][kk];
                }
            }

            cache_rows[rr] = row;
        }

        cached_cnt = new_cnt;
    }

    return cache_rows[r][k];
}
