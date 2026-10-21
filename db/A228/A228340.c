/*
 * A228340  Triangle read by rows: T(n,k) = (n-1)*T(n-1,k) + T(n-2,k), with
 *          T(n,n-1)=1, T(n,n-2)=n-2, for n >= 1, 0 <= k <= n-1.
 * https://oeis.org/A228340
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   not stated
 * Range:     u8 n <= 21, u16 n <= 45, u32 n <= 92, u64 n <= 231,
 *            u128 n <= 630, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A228340:
   Triangle read by rows.
   Recurrence:
     T(1,0) = 1
     T(2,0) = 0,  T(2,1) = 1
     For n ≥ 3 and 0 ≤ k ≤ n−1:
       if k == n-1      : T(n,k) = 1
       else if k == n-2 : T(n,k) = n-2
       else             : T(n,k) = (n-1)*T(n-1,k) + T(n-2,k)
   a(n) is the nth element of the triangle when read row‑wise
   (row 1, row 2, …).  Indexing starts at 1 (a(1)=1). */

#include <stdlib.h>
#include <stddef.h>

static seq_t **triangle = NULL;   /* triangle[i] points to row i, rows 1..max_row */
static size_t  max_row   = 0;     /* highest row currently stored */

/* Ensure that rows 1 .. r are allocated and filled.  Rows already built
   are left untouched.  If an allocation fails, the partially built
   state stays unchanged, and subsequent calls may fail with 0. */
static void
ensure_rows(idx_t r)
{
    if (r <= (idx_t)max_row)
        return;                       /* nothing to do */

    /* Grow the row pointer array if necessary */
    size_t new_cap = (size_t)(r + 1);   /* positions 0 .. r, 0 unused */
    if (triangle == NULL) {
        triangle = (seq_t**)malloc(new_cap * sizeof(seq_t*));
    } else {
        triangle = (seq_t**)realloc(triangle, new_cap * sizeof(seq_t*));
    }
    if (triangle == NULL)            /* allocation failed */
        return;

    /* Allocate and compute the missing rows */
    for (size_t i = max_row + 1; i <= (size_t)r; ++i) {
        seq_t *row = (seq_t*)malloc(i * sizeof(seq_t));
        if (row == NULL)          /* allocation failed – leave state as is */
            return;

        if (i == 1) {                 /* row 1 */
            row[0] = (seq_t)1;
        } else if (i == 2) {          /* row 2 */
            row[0] = (seq_t)0;
            row[1] = (seq_t)1;
        } else {                      /* n ≥ 3 */
            seq_t *prev1 = triangle[i-1];
            seq_t *prev2 = triangle[i-2];

            /* last two entries are fixed */
            row[i-1] = (seq_t)1;
            row[i-2] = (seq_t)(i-2);

            /* interior entries */
            for (size_t k = 0; k < i-2; ++k) {
                seq_t term = ((seq_t)(i-1)) * prev1[k];
                row[k] = term + prev2[k];
            }
        }
        triangle[i] = row;
    }
    max_row = (size_t)r;
}

/* Return the n-th element of the triangle (1‑based). */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;               /* undefined, return 0 for safety */

    /* Find the row r such that the cumulative number of elements up
       to that row is at least n. */
    idx_t r = 0;
    idx_t cumulative = 0;
    while (cumulative < n) {
        ++r;
        cumulative = (r * (r + 1)) / 2;     /* integer arithmetic */
    }

    /* Position k within that row (0‑based). */
    idx_t before = ((r - 1) * r) / 2;
    idx_t k = n - before - 1;

    ensure_rows(r);
    if (triangle == NULL || triangle[r] == NULL)
        return (seq_t)0;               /* allocation failed */

    return triangle[(size_t)r][(size_t)k];
}
