/*
 * A144382  T(1,k) = -1 and T(n,k) = [t^k] 1/(-1 + t - t^n) for n >= 2, square
 *          array read by ascending antidiagonals (n >= 1, k >= 0).
 * https://oeis.org/A144382
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, easy, tabl
 * Formula:   not stated
 * Range:     i8 n <= 78, i16 n <= 78, i32 n <= 78, i64 n <= 78, i128 n <= 78,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A144382:
    T(1,k) = -1                                 
    For n≥2:  T(n,0) = -1
              T(n,k) - T(n,k-1) + T(n,k-n) = 0
    a(n) is read from the square array T(n,k) by
    ascending antidiagonals (n+k fixed, k increasing).

    This function follows that definition.  The mapping
    from index n to the pair (row, column) is:
          row = s - k + 1 ,
          k   = n - s(s+1)/2 - 1 ,
    where s is the greatest integer with s(s+1)/2 < n.
*/

#include <stdlib.h>
#include <stdint.h>
#include <inttypes.h>

/* --- Helper structure for cached rows ----------------------------------- */
typedef struct {
    seq_t *coeff;   /* coefficient array for this row */
    size_t cap;     /* number of allocated entries (k+1) */
} RowEntry;

static RowEntry **row_cache = NULL; /* array of pointers to RowEntry, indexed by row number */
static size_t row_cache_size = 0;   /* current allocated size of row_cache array */

/* --- Find the largest s with s(s+1)/2 < n ------------------------------- */
static idx_t find_s(idx_t n)
{
    /* Search range: s ∈ [0, 2^33]   (s*(s+1)/2 < 2^64 guaranteed) */
    idx_t lo = 0;
    idx_t hi = ((idx_t)1 << 33);   /* 8.6e9, safely above the needed maximum */

    while (lo < hi) {
        idx_t mid = lo + (hi - lo + 1) / 2;
        __int128 tri = (__int128)mid * (mid + 1) / 2;
        if (tri < n)
            lo = mid;
        else
            hi = mid - 1;
    }
    return lo;
}

/* --- Get coefficient T(row,k) with caching ------------------------------ */
static seq_t get_T(idx_t row, idx_t k)
{
    if (row == 1)
        return (seq_t)-1;      /* T(1,k) = -1 for all k */

    /* Ensure row_cache has space for this row */
    if (row_cache_size <= (size_t)row) {
        size_t new_size = (size_t)row + 1;
        RowEntry **new_cache = (RowEntry **)realloc(row_cache, new_size * sizeof(RowEntry *));
        if (!new_cache) exit(EXIT_FAILURE);   /* allocation failure */
        /* initialize new slots to NULL */
        for (size_t i = row_cache_size; i < new_size; ++i) new_cache[i] = NULL;
        row_cache = new_cache;
        row_cache_size = new_size;
    }

    RowEntry *RE = row_cache[(size_t)row];
    if (!RE) {
        RE = (RowEntry *)malloc(sizeof(RowEntry));
        if (!RE) exit(EXIT_FAILURE);
        RE->coeff = NULL;
        RE->cap   = 0;
        row_cache[(size_t)row] = RE;
    }

    /* If cache not large enough, extend it */
    if (RE->cap <= (size_t)k) {
        size_t old_cap = RE->cap;
        size_t new_cap = (size_t)k + 1;
        seq_t *new_coeff = (seq_t *)realloc(RE->coeff, new_cap * sizeof(seq_t));
        if (!new_coeff) exit(EXIT_FAILURE);
        RE->coeff = new_coeff;
        /* Fill new entries */
        for (size_t i = old_cap; i < new_cap; ++i) {
            if (i == 0)
                RE->coeff[i] = (seq_t)-1;
            else if (i < (size_t)row)
                RE->coeff[i] = (seq_t)-1;
            else
                RE->coeff[i] = RE->coeff[i-1] - RE->coeff[i-(size_t)row];
        }
        RE->cap = new_cap;
    }

    return RE->coeff[(size_t)k];
}

/* --- Main function ------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0)   /* should not happen, but keep defined */
        return (seq_t)0;

    /* determine diagonal s such that s(s+1)/2 < n <= (s+1)(s+2)/2 */
    idx_t s = find_s(n);
    /* number of terms before this diagonal */
    __int128 tri_s = (__int128)s * (s + 1) / 2;
    idx_t k = n - (idx_t)tri_s - 1;   /* column index (0‑based) */
    idx_t row = s - k + 1;            /* row index (1‑based) */

    return get_T(row, k);
}
