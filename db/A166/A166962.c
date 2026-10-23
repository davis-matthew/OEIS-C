/*
 * A166962  Triangle T(n,k) read by rows: T(n,1) = T(n,n)=1, otherwise T(n,k)
 *          = (3n-3k+1)*T(n-1,k-1) + k*(3k-2)*T(n-1,k), 1<=k<=n.
 * https://oeis.org/A166962
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, tabl
 * Formula:   not stated
 * Range:     u8 n <= 11, u16 n <= 23, u32 n <= 47, u64 n <= 96, u128 n <= 259,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Triangle T(n,k) of A166962:
 *   T(n,1) = T(n,n) = 1,
 *   T(n,k) = (3*n-3*k+1)*T(n-1,k-1) + k*(3*k-2)*T(n-1,k)  (1 < k < n).
 * The sequence a(n) is the triangle read by rows:
 * a(1)=T(1,1), a(2)=T(2,1), a(3)=T(2,2), a(4)=T(3,1), ...
 */

#ifdef __cplusplus
#define SEQ_ALLOC(count)     new seq_t[count]
#define SEQ_FREE(ptr)        delete[] ptr
#else
#define SEQ_ALLOC(count)     ((seq_t*)malloc((count)*sizeof(seq_t)))
#define SEQ_FREE(ptr)        free(ptr)
#endif

/* run‑time cache of the triangle rows */
static seq_t **triangle = NULL;   /* triangle[n] points to row n, indices 0..n */
static size_t tri_rows   = 0;      /* highest row computed (0 means none) */
static size_t tri_cap    = 0;      /* capacity of the triangle pointer array */

static void ensure_tri_cap(size_t needed)
{
    if (needed <= tri_cap) return;
    size_t new_cap = tri_cap ? tri_cap * 2 : 64;
    while (new_cap < needed) new_cap *= 2;
    seq_t **new_arr = (seq_t**)realloc(triangle, new_cap * sizeof(seq_t *));
    if (!new_arr) abort();          /* memory exhausted */
    triangle = new_arr;
    tri_cap = new_cap;
}

static void compute_rows_to(size_t target)
{
    if (target <= tri_rows) return;
    ensure_tri_cap(target + 1);      /* allow indices up to target */

    for (size_t n = tri_rows + 1; n <= target; ++n) {
        seq_t *row = SEQ_ALLOC(n + 1);   /* indices 0..n, we use 1..n */
        if (!row) abort();
        row[1] = 1;
        row[n] = 1;

        if (n > 1) {
            seq_t *prev = triangle[n - 1];
            for (size_t k = 2; k < n; ++k) {
                seq_t term1 = (seq_t)(3 * n - 3 * k + 1) * prev[k - 1];
                seq_t term2 = (seq_t)k * (seq_t)(3 * k - 2) * prev[k];
                row[k] = term1 + term2;
            }
        }
        triangle[(size_t)n] = row;
        tri_rows = n;
    }
}

/* public entry point */
seq_t a(idx_t n)
{
    if (n == 0) return 0;             /* out of scope, but safe */

    /* find the row r with 1+2+...+r >= n */
    idx_t cum = 0;
    idx_t r   = 0;
    while (cum < n) {
        ++r;
        cum += r;
    }
    /* column k within that row */
    idx_t k = n - (cum - r);       /* 1 <= k <= r */

    compute_rows_to((size_t)r);      /* ensure row r exists */
    return triangle[(size_t)r][(size_t)k];
}
