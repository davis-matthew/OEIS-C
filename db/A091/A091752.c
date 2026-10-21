/*
 * A091752  Generalized Stirling2 array (-1,2)S2. Irregular triangle a(n, m)
 *          for n >= 1 and 2 <= m <= 2*n.
 * https://oeis.org/A091752
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, easy, tabf
 * Formula:   not stated
 * Range:     i8 n <= 9, i16 n <= 16, i32 n <= 36, i64 n <= 41, i128 n <= 41,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A091752: flattened irregular triangle a(n,m)
 * a(n,m) = a(n-1,m-2) - 2*(3*n-(m+2))*a(n-1,m-1)
 *          + (3*n-(m+3))*(3*n-(m+2))*a(n-1,m),
 * with a(1,2)=1 and a(1,m)=0 for m≠2.
 * The 1‑D sequence a(k) is obtained by reading the triangle left‑to‑right,
 * rows of length 1,3,5,… (i.e. all m=2…2*n for row n).  For k≥1 let
 * r=ceil(√k), m = (k-(r-1)²)+1 + 1.  Then a(k)=a(r,m). This implementation
 * memoises computed rows and is O(n) with respect to r. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* ------------------------------------------------------------------ */
/* integer square‑root (floor) – does not overflow                     */
/* ------------------------------------------------------------------ */
static idx_t floor_sqrt_u64(idx_t n)
{
    idx_t lo = 0, hi = n, ans = 0;
    while (lo <= hi) {
        idx_t mid = lo + ((hi - lo) >> 1);
        if (mid == 0) { ans = 0; lo = 1; continue; }
        if (mid > n / mid) {
            hi = mid - 1;
        } else {
            ans = mid;
            lo = mid + 1;
        }
    }
    return ans;
}

/* ------------------------------------------------------------------ */
/* Cache for computed rows of the triangle                            */
/* ------------------------------------------------------------------ */
static seq_t **ROWS = NULL;     /* array of pointers to rows      */
static idx_t ROWS_CAP = 0;      /* current capacity of ROWS       */
static idx_t MAX_COMPUTED = 0;  /* rows [1..MAX_COMPUTED] computed */

/* ------------------------------------------------------------------ */
/* Ensure that the global array can hold indices up to r              */
/* ------------------------------------------------------------------ */
static void ensure_capacity(idx_t r)
{
    if (ROWS == NULL) {
        ROWS_CAP = 1;
        ROWS = (seq_t **)calloc(ROWS_CAP, sizeof(seq_t *));
        if (!ROWS) abort();
    }
    if (ROWS_CAP <= r) {
        size_t newcap = ROWS_CAP * 2;
        if (newcap <= (size_t)r) newcap = (size_t)r + 1;
        seq_t **tmp = (seq_t **)realloc(ROWS, newcap * sizeof(seq_t *));
        if (!tmp) abort();
        /* set new pointers to NULL */
        for (size_t i = ROWS_CAP; i < newcap; ++i) tmp[i] = NULL;
        ROWS = tmp;
        ROWS_CAP = newcap;
    }
}

/* ------------------------------------------------------------------ */
/* Compute a single row i (i >= 1) and store it in ROWS[i]             */
/* ------------------------------------------------------------------ */
static void compute_row(idx_t i)
{
    idx_t len = 2 * i + 1;                    /* indices 0..2*i          */
    seq_t *curr = (seq_t *)calloc(len, sizeof(seq_t));
    if (!curr) abort();

    if (i == 1) {
        curr[2] = (seq_t)1;                  /* a(1,2)=1 */
        ROWS[i] = curr;
        return;
    }

    seq_t *prev = ROWS[i - 1];
    idx_t prev_len = 2 * (i - 1) + 1;       /* indices 0..2*(i-1)     */

    for (idx_t m = 2; m <= 2 * i; ++m) {
        seq_t a12 = 0, a11 = 0, a10 = 0;
        if (m >= 2 && m - 2 < prev_len) a12 = prev[m - 2];
        if (m >= 1 && m - 1 < prev_len) a11 = prev[m - 1];
        if (m < prev_len)                a10 = prev[m];

        seq_t term = a12;
        term -= (seq_t)2 * (seq_t)(3 * i - (m + 2)) * a11;
        seq_t t1 = (seq_t)(3 * i - (m + 3));
        seq_t t2 = (seq_t)(3 * i - (m + 2));
        term += t1 * t2 * a10;

        curr[m] = term;
    }

    ROWS[i] = curr;
}

/* ------------------------------------------------------------------ */
/* Main sequence accessor                                               */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;              /* undefined below offset */

    /* Find the row r such that (r-1)^2 < n ≤ r^2  (i.e. r = ceil(sqrt(n))) */
    idx_t r = floor_sqrt_u64(n);
    if (r * r < n) ++r;                       /* ceil(sqrt(n))          */

    /* Compute all rows up to r if necessary */
    ensure_capacity(r);
    for (idx_t i = MAX_COMPUTED + 1; i <= r; ++i)
        compute_row(i);
    MAX_COMPUTED = r;

    /* Column m inside row r */
    idx_t prev_sq = (r - 1) * (r - 1);
    idx_t pos_in_row = n - prev_sq;          /* 1‑based position in row */
    idx_t m = pos_in_row + 1;                /* m = 2 + (pos-1)        */

    return ROWS[r][m];
}
