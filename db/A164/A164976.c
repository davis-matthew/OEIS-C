/*
 * A164976  Triangle read by rows, expansion of 1/(1-2*y*x-x+x^2-y*x^2).
 * https://oeis.org/A164976
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  tabl, sign
 * Formula:   a(n) is the nth element (row‑wise) of the triangle T(r,c)
 *            defined by the recurrence T(r,c) = 2*T(r-1,c-1) + T(r-1,c) -
 *            T(r-2,c) + T(r-2,c-1) with T(0,0)=1 and T(r,c)=0 when c<0 or
 *            c>r. This triangle expands 1/(1-2*y*x-x+x^2-y*x^2). a(n) is
 *            obtained by finding the row r such that r*(r+1)/2 ≤ n < (...
 * Range:     i8 n <= 24, i16 n <= 72, i32 n <= 245, i64 n <= 891,
 *            i128 n <= 3463, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

static seq_t *tri = NULL;        /* linearized triangle */
static size_t tri_cap = 0;       /* allocated capacity */
static idx_t last_row = 0;       /* last fully computed row */

/* Return the n-th element of A164976 */
seq_t a(idx_t n)
{
    /* Find the row r that contains the nth element */
    idx_t r = 0;
    while ((r + 1) * (r + 2) / 2 <= n)
        ++r;
    idx_t c = n - r * (r + 1) / 2;   /* column within the row */

    /* Ensure the cache is large enough */
    idx_t needed = (r + 1) * (r + 2) / 2;
    if (!tri) {
        tri = (seq_t *)malloc((size_t)needed * sizeof(seq_t));
        tri_cap = (size_t)needed;
        /* seed first element */
        tri[0] = (seq_t)1;
        last_row = 0;
    } else if ((size_t)needed > tri_cap) {
        seq_t *tmp = (seq_t *)realloc(tri, (size_t)needed * sizeof(seq_t));
        if (!tmp) return (seq_t)0;   /* allocation failed; return 0 */
        tri = tmp;
        tri_cap = (size_t)needed;
    }

    /* Compute rows up to r (if not already computed) */
    if (r > last_row) {
        for (idx_t i = last_row + 1; i <= r; ++i) {
            for (idx_t j = 0; j <= i; ++j) {
                size_t idx = (size_t)i * (i + 1) / 2 + j;
                if (i == 0) {          /* only (0,0) occurs in row 0 */
                    tri[idx] = (seq_t)1;
                    continue;
                }
                seq_t val = 0;
                if (i >= 1 && j <= i - 1)
                    val += tri[(i - 1) * i / 2 + j];
                if (i >= 1 && j >= 1 && j <= i)
                    val += (seq_t)2 * tri[(i - 1) * i / 2 + (j - 1)];
                if (i >= 2 && j <= i - 2)
                    val -= tri[(i - 2) * (i - 1) / 2 + j];
                if (i >= 2 && j >= 1 && j <= i - 1)
                    val += tri[(i - 2) * (i - 1) / 2 + (j - 1)];
                tri[idx] = val;
            }
        }
        last_row = r;
    }

    return tri[(size_t)r * (r + 1) / 2 + c];
}
