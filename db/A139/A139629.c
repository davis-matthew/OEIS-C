/*
 * A139629  Row sums of A139624, number of connected directed multigraphs with
 *          loops and no vertex of degree 0, with n arcs, which are transitive
 *          (the existence of a path between two points implies the existence
 *          of an arc between those two points).
 * https://oeis.org/A139629
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  more, nonn
 * Formula:   a(0) = 1; for n ≥ 1: a(n) = 3 * a(n-1) if n is even a(n) = 3 *
 *            a(n-1) – Catalan((n-1)/2) if n is odd Catalan numbers: C_0 = 1;
 *            C_{k+1} = C_k * 2*(2*k+1)/(k+2) (k ≥ 0)
 * Range:     u8 n <= 3, u16 n <= 3, u32 n <= 3, u64 n <= 3, u128 n <= 3,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#ifdef __cplusplus
# include <algorithm>
#endif

/* ------------------------------------------------------------------ *\
   Static caches for sequence values and Catalan numbers
\* ------------------------------------------------------------------ */
static seq_t *a_cache = NULL;      /* a(n) values          */
static size_t a_cap   = 0;          /* allocated capacity   */
static size_t a_used  = 0;          /* number of a values   */

static seq_t *cat_cache = NULL;    /* Catalan numbers C_k   */
static size_t cat_cap   = 0;        /* allocated capacity   */
static size_t cat_used  = 0;        /* number of C_k values */

/* ------------------------------------------------------------------ *\
   Resize an array storing seq_t objects
\* ------------------------------------------------------------------ */
static void resize_array(seq_t **arr, size_t *cap, size_t used_count, size_t new_cap)
{
    if (new_cap <= *cap) return;           /* already sufficient */

#if defined(__cplusplus)
    /* C++: allocate new array, copy old elements, delete old array */
    seq_t *tmp = new seq_t[new_cap];
    for (size_t i = 0; i < used_count; ++i) tmp[i] = (*arr)[i];
    delete[] *arr;
    *arr = tmp;
#else
    /* C: realloc is fine for trivial types */
    *arr = (seq_t*)realloc(*arr, new_cap * sizeof(seq_t));
#endif
    *cap = new_cap;
}

/* ------------------------------------------------------------------ *\
   Ensure that a_cache and cat_cache contain values up to n (inclusive)
\* ------------------------------------------------------------------ */
static void ensure_values(idx_t n)
{
    /* ----------------------------------------------------------------
       Resize a_cache if necessary
       ---------------------------------------------------------------- */
    if (a_used <= (size_t)n) {
        size_t new_cap = a_cap ? a_cap : 8;     /* start with a small block */
        while (new_cap <= (size_t)n) new_cap <<= 1;
        resize_array(&a_cache, &a_cap, a_used, new_cap);
    }

    /* ----------------------------------------------------------------
       Resize cat_cache if necessary.  The largest Catalan index needed
       for any odd n ≤ current n is floor(n/2).  Using that bound keeps
       the cache just large enough.
       ---------------------------------------------------------------- */
    size_t max_cat_idx = n / 2;
    if (cat_used <= max_cat_idx) {
        size_t new_cap = cat_cap ? cat_cap : 8;
        while (new_cap <= max_cat_idx) new_cap <<= 1;
        resize_array(&cat_cache, &cat_cap, cat_used, new_cap);
    }

    /* ----------------------------------------------------------------
       Initialize base values if not yet done
       ---------------------------------------------------------------- */
    if (cat_used == 0) {
        cat_cache[0] = (seq_t)1;   /* C_0 = 1 */
        cat_used = 1;
    }
    if (a_used == 0) {
        a_cache[0] = (seq_t)1;   /* a(0) = 1 */
        a_used = 1;
    }

    /* ----------------------------------------------------------------
       Compute missing Catalan numbers
       ---------------------------------------------------------------- */
    for (size_t k = cat_used; k <= max_cat_idx; ++k) {
        /* C_k = C_{k-1} * 2*(2*(k-1)+1) / (k+1)  (k ≥ 1) */
        seq_t num = cat_cache[k-1] * (seq_t)2 *
                    ((seq_t)2 * (seq_t)(k-1) + (seq_t)1);
        seq_t next = num / (seq_t)(k + 1);
        cat_cache[k] = next;
    }
    cat_used = max_cat_idx + 1;

    /* ----------------------------------------------------------------
       Compute missing a(n) values
       ---------------------------------------------------------------- */
    for (size_t i = a_used; i <= (size_t)n; ++i) {
        seq_t val = a_cache[i-1] * (seq_t)3;
        if (i & 1) {                     /* odd i */
            size_t cat_idx = (i - 1) >> 1;
            val -= cat_cache[cat_idx];
        }
        a_cache[i] = val;
    }
    a_used = (size_t)n + 1;
}

/* ------------------------------------------------------------------ *\
   External API
\* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    ensure_values(n);
    return a_cache[(size_t)n];
}
