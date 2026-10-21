/*
 * A086764  Triangle T(n, k), read by row, related to Euler's difference table
 *          A068106 (divide column k of A068106 by k!).
 * https://oeis.org/A086764
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 20, u16 n <= 44, u32 n <= 104, u64 n <= 230,
 *            u128 n <= 629, big any n
 * Helpers:   mul_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A086764
 *  Sequence defined from the triangle T(n,k) (n>=0, 0<=k<=n):
 *      T(n,n)   = 1
 *      T(n,0)   = 0   for n>0
 *      T(n,k)   = (k+1)*T(n,k+1) - T(n-1,k)   (0<=k<n)
 *  a(n) is the triangle read row‑wise, starting with a(0)=1.
 *  The function builds rows of the triangle on demand and memoises
 *  them; it works for any seq_t large enough to hold the value.    */

#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>
#include "../utils/mul_small.h"

/* global storage for triangle rows */
static seq_t **row_ptrs = NULL;
static size_t rows_cnt = 0;

/* integer square root: floor(sqrt(x))  for 0 <= x <= 2^64-1   */
static idx_t floor_sqrt_u64(idx_t x)
{
    idx_t lo   = 0;
    idx_t hi   = (idx_t)1 << 32;               /* 2^32^2 > 2^64        */
    idx_t ans  = 0;
    while (lo <= hi) {
        idx_t mid = lo + ((hi - lo) >> 1);
        __int128 sq = (__int128)mid * mid;
        if (sq <= x) {
            ans = mid;
            lo  = mid + 1;
        } else {
            if (mid == 0) break;
            hi = mid - 1;
        }
    }
    return ans;
}

/* ensure that row_ptrs has room for at least 'needed' rows */
static void grow_rows(size_t needed)
{
    if (needed <= 0) return;
    static size_t cap = 0;
    if (cap >= needed) return;
    size_t new_cap = cap ? cap * 2 : 4;
    while (new_cap < needed) new_cap <<= 1;
    seq_t **tmp = (seq_t**)realloc(row_ptrs, new_cap * sizeof(seq_t*));
    row_ptrs = tmp;
    cap = new_cap;
}

/* compute all triangle rows up to and including row 'r' */
static void compute_row(size_t r)
{
    if(rows_cnt > r) return;                 /* already available        */

    grow_rows(r + 1);                         /* make sure the array fits */

    /* row 0 */
    if (rows_cnt == 0) {
        seq_t *row0 = (seq_t*)calloc(1, sizeof(seq_t));
        row0[0] = (seq_t)1;
        row_ptrs[0] = row0;
        rows_cnt = 1;
    }

    /* subsequent rows */
    for (size_t cur = rows_cnt; cur <= r; ++cur) {
        size_t len = cur + 1;
        seq_t *newrow = (seq_t*)calloc(len, sizeof(seq_t));

        /* T(cur,cur) = 1 */
        newrow[cur] = (seq_t)1;

        if (cur == 0) {          /* only row 0 was handled above */
            row_ptrs[cur] = newrow;
            continue;
        }

        seq_t *prev = row_ptrs[cur - 1];
        /* compute T(cur,k) for k = cur-1 .. 0  */
        for (size_t k = cur - 1; ; --k) {
            seq_t term = mul_small(newrow[k + 1], (unsigned int)(k + 1));
            newrow[k] = term - prev[k];
            if (k == 0) break;
        }
        row_ptrs[cur] = newrow;
    }
    rows_cnt = r + 1;
}

/* ------------------------------------------------------------ */

seq_t a(idx_t n)
{
    /* find row r and column k for index n, using 0‑based indexing */
    idx_t val = 8ull * n + 1ull;             /* 8n+1  */
    idx_t s   = floor_sqrt_u64(val);
    idx_t r   = (s - 1) / 2;                 /* row index     */
    idx_t offset = r * (r + 1) / 2;          /* first index of row r */
    idx_t k   = n - offset;                  /* column in row */

    compute_row((size_t)r);                  /* ensure row exists */

    return row_ptrs[(size_t)r][(size_t)k];
}
