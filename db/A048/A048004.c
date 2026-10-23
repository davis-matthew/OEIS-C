/*
 * A048004  Triangular array read by rows: T(n,k) = number of binary vectors
 *          of length n whose longest run of consecutive 1's has length k, for
 *          n >= 0, 0 <= k <= n.
 * https://oeis.org/A048004
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, nice
 * Formula:   a(n) = T(r,c) where T(n,k) = number of binary vectors of length
 *            n whose longest run of consecutive 1's has length k. The
 *            triangle is read by rows (row 0: k=0; row 1: k=0,1; …).
 *            Recurrence for interior entries: T(n,k) = 2*T(n-1,k) +
 *            T(n-1,k-1) - 2*T(n-2,k-1) + T(n-k-1,k-1) - T(n-k-2,k) with
 *            bounda...
 * Range:     u8 n <= 56, u16 n <= 173, u32 n <= 598, u64 n <= 2215,
 *            u128 n <= 8520, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* ------------------------------------------------------------------ */
/* Integer square root for idx_t (fits in uint64_t).                     */
/* ------------------------------------------------------------------ */
static idx_t isqrt_u64(idx_t x)
{
    idx_t lo = 0, hi = (x >> 1) + 1;
    while (lo < hi) {
        idx_t mid = lo + ((hi - lo + 1) >> 1);
        __int128 sq = (__int128)mid * mid;
        if (sq <= x)
            lo = mid;
        else
            hi = mid - 1;
    }
    return lo;
}

/* ------------------------------------------------------------------ */
/* Storage for the computed triangle rows                              */
/* ------------------------------------------------------------------ */
static seq_t **row_ptrs      = NULL;   /* array of pointers to rows  */
static idx_t   rows_computed = 0;      /* highest row index fully computed (0‑based) */

/* ------------------------------------------------------------------ */
/* Ensure that all rows up to 'need' are computed and stored.          */
/* ------------------------------------------------------------------ */
static void ensure_rows(idx_t need)
{
    /* Grow the array of row pointers if necessary */
    if (row_ptrs == NULL) {
        row_ptrs = (seq_t **)malloc((need + 1) * sizeof(seq_t *));
        if (row_ptrs == NULL) abort();
    } else if (need >= rows_computed) {
        seq_t **tmp = (seq_t **)realloc(row_ptrs,
                                        (need + 1) * sizeof(seq_t *));
        if (tmp == NULL) abort();
        row_ptrs = tmp;
    }

    /* compute missing rows sequentially */
    for (idx_t n = rows_computed; n <= need; ++n) {
        /* allocate storage for this row */
        row_ptrs[n] = (seq_t *)malloc((n + 1) * sizeof(seq_t));
        if (row_ptrs[n] == NULL) abort();

        /* fill each column k */
        for (idx_t k = 0; k <= n; ++k) {
            seq_t val = 0;
            if (k == 0 || k == n) {
                val = 1;
            } else {
                /* T(n-1,k)  */
                seq_t t1 = (n >= 1 && k <= n-1) ? row_ptrs[n-1][k] : 0;
                /* T(n-1,k-1) */
                seq_t t2 = (n >= 1 && k-1 <= n-1) ? row_ptrs[n-1][k-1] : 0;
                /* T(n-2,k-1) */
                seq_t t3 = (n >= 2 && k-1 <= n-2) ? row_ptrs[n-2][k-1] : 0;
                /* T(n-k-1,k-1) */
                seq_t t4 = (n >= k+1 && k-1 <= n-k-1) ? row_ptrs[n-k-1][k-1] : 0;
                /* T(n-k-2,k) */
                seq_t t5 = (n >= k+2 && k <= n-k-2) ? row_ptrs[n-k-2][k] : 0;

                val = 2 * t1 + t2 - 2 * t3 + t4 - t5;
            }
            row_ptrs[n][k] = val;
        }
    }
    rows_computed = need + 1;  /* rows 0 .. need are now available */
}

/* ------------------------------------------------------------------ */
/* Public function required by the harness                            */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    /* Find the row r containing index n in the flattened array */
    idx_t r = (isqrt_u64(8 * n + 1) - 1) / 2;
    idx_t start = r * (r + 1) / 2;      /* first index of row r */
    idx_t k = n - start;                /* column within row r */

    /* Compute any missing rows up to r */
    if (r >= rows_computed)
        ensure_rows(r);

    return row_ptrs[r][k];
}
