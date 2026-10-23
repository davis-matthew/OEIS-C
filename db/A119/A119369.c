/*
 * A119369  Pendular trinomial triangle, read by rows of 2n+1 terms (n>=0),
 *          defined by the recurrence: if 0 < k < n, T(n,k) = T(n-1,k) +
 *          T(n,2n-1-k); otherwise, if n-1 < k < 2n-1, T(n,k) = T(n-1,k) +
 *          T(n,2n-2-k); with T(n,0)=T(n+1,2n)=1 and
 *          T(n+1,2n+1)=T(n+1,2n+2)=0.
 * https://oeis.org/A119369
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   not stated
 * Range:     u8 n <= 54, u16 n <= 130, u32 n <= 378, u64 n <= 1256,
 *            u128 n <= 2600, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Pendular trinomial triangle
 *
 * The triangle T(n,k) (0 ≤ k ≤ 2n) is defined by
 *   T(0,0) = 1
 *   T(n,0) = 1          for n ≥ 1
 *   T(n,2n-1) = 0       for n ≥ 1
 *   T(n,2n)   = 0       for n ≥ 1
 *   for 0 < k < n  : T(n,k) = T(n-1,k) + T(n,2n-1-k)
 *   for n-1 < k < 2n-1: T(n,k) = T(n-1,k) + T(n,2n-2-k)
 *
 * The sequence a(n) is obtained by reading the triangle
 * row‑wise (row 0, then row 1, etc.).  For an index n,
 * let r = floor(sqrt(n)) and p = n - r^2 (0 ≤ p ≤ 2r); then
 *     a(n) = T(r,p).
 *
 * The code below memoises computed triangle entries in
 * dynamically growing arrays.  It runs in O(n) time per
 * call and uses only integer arithmetic other than allocations.
 */

#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>

/*  Integer square root: floor(sqrt(n)) for 0 ≤ n ≤ UINT64_MAX.  */
static idx_t isqrt_u64(idx_t n)
{
    idx_t low = 0, high = n;
    while (low < high) {
        idx_t mid = (low + high + 1) >> 1;
        if (mid <= n / mid)      /* mid*mid ≤ n without overflow   */
            low = mid;
        else
            high = mid - 1;
    }
    return low;
}

/*  Storage for computed rows.
 *  Tvals[n] points to an array of length 2*n+1 holding T(n,k).
 *  Tcomp[n] is a bool array marking whether the entry has been computed.  */
static seq_t **Tvals   = NULL;
static bool **Tcomp   = NULL;
static idx_t max_row_allocated = 0;

/*  Allocate row pointers up to 'needed' (inclusive).  */
static void ensure_rows(idx_t needed)
{
    /* First allocation case: needed is 0 (first row).   */
    if (Tvals == NULL) {
        Tvals = (seq_t**)malloc((needed + 1) * sizeof(seq_t*));
        Tcomp = (bool**)malloc((needed + 1) * sizeof(bool*));
        if (!Tvals || !Tcomp) abort();
        max_row_allocated = needed;
    }
    else if (needed > max_row_allocated) {
        Tvals = (seq_t**)realloc(Tvals, (needed + 1) * sizeof(seq_t*));
        Tcomp = (bool**)realloc(Tcomp, (needed + 1) * sizeof(bool*));
        if (!Tvals || !Tcomp) abort();
        max_row_allocated = needed;
    }

    /* Allocate any newly required rows.  */
    for (idx_t r = (max_row_allocated == 0 ? 0 : 1); r <= needed; ++r) {
        if (Tvals[r] == NULL) {
            size_t len = (size_t)(2 * r + 1);
            Tvals[r] = (seq_t*)calloc(len, sizeof(seq_t));
            if (!Tvals[r]) abort();
            Tcomp[r] = (bool*)calloc(len, sizeof(bool));
            if (!Tcomp[r]) abort();
        }
    }
}

/*  Recursive computation with memoisation.  */
static seq_t get_T(idx_t n, idx_t k)
{
    if (Tcomp[n][k])
        return Tvals[n][k];

    seq_t val;
    if (n == 0) {
        /* Only valid index is k == 0.  */
        val = 1;
    }
    else if (k == 0) {
        val = 1;
    }
    else if (k == 2 * n - 1 || k == 2 * n) {
        val = 0;
    }
    else if (k < n) {
        val = get_T(n - 1, k) + get_T(n, 2 * n - 1 - k);
    }
    else { /* n-1 < k < 2n-1 */
        val = get_T(n - 1, k) + get_T(n, 2 * n - 2 - k);
    }

    Tvals[n][k] = val;
    Tcomp[n][k] = true;
    return val;
}

/*  Public sequence function.  */
seq_t a(idx_t n)
{
    /* Determine which row and position within that row the
     * index n corresponds to.  */
    idx_t row = isqrt_u64(n);
    idx_t pos = n - row * row;           /* 0 … 2*row */

    /* Ensure the required rows are allocated.  */
    if (row > max_row_allocated)
        ensure_rows(row);
    else if (Tvals == NULL)
        ensure_rows(row);

    return get_T(row, pos);
}
