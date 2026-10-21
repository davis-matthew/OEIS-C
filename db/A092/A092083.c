/*
 * A092083  A convolution triangle of numbers obtained from A034789.
 * https://oeis.org/A092083
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, tabl
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 10, u32 n <= 28, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A092083: convolution triangle of A034789
   a(n,m) = 6*(6*(n-1)+m)*a(n-1,m)/n + m*a(n-1,m-1)/n   (n>=m>=1)
   a(1,1) = 1 ; a(n,0)=0 ; a(n,m)=0 for n<m.
   a(n) is obtained by reading the triangle row‑by‑row left to right. */
#include <stdlib.h>

/* Static cache of computed triangle rows */
static seq_t **tri = NULL;
static idx_t  maxRow = 0;   /* highest row already computed */

/* Ensure the triangle is computed up to at least row 'need' */
static void ensure_row(idx_t need)
{
    if (need <= maxRow) return;

    /* Allocate or grow array of row pointers */
    if (tri == NULL) {
        tri = (seq_t**)malloc((size_t)(need + 1) * sizeof(seq_t*));
    } else {
        tri = (seq_t**)realloc(tri, (size_t)(need + 1) * sizeof(seq_t*));
    }

    /* Allocate each new row */
    for (idx_t r = maxRow + 1; r <= need; ++r) {
        tri[r] = (seq_t*)malloc((size_t)(r + 1) * sizeof(seq_t));
        tri[r][0] = (seq_t)0;  /* unused element */
    }

    /* Base row (n=1) */
    if (maxRow == 0 && need >= 1) {
        tri[1][1] = (seq_t)1;
    }

    /* Compute new rows */
    for (idx_t r = (maxRow > 0 ? maxRow : 1) + 1; r <= need; ++r) {
        for (idx_t c = 1; c <= r; ++c) {
            seq_t val = 0;

            /* Obtain previous values (if they exist) */
            seq_t prev1 = 0;
            if (c <= r - 1) prev1 = tri[r - 1][c];
            seq_t prev2 = (c > 1) ? tri[r - 1][c - 1] : (seq_t)0;

            /* Compute numerator of recurrence:
               X = 6*(6*(r-1)+c) */
            seq_t X = (seq_t)6 *
                      ((seq_t)6 * (seq_t)(r - 1) + (seq_t)c);

            seq_t num1 = prev1 * X;
            seq_t num2 = prev2 * (seq_t)c;
            seq_t num  = num1 + num2;

            val = num / (seq_t)r;   /* exact division */
            tri[r][c] = val;
        }
    }

    if (maxRow == 0 && need >= 1) { /* set base if not yet set */
        tri[1][1] = (seq_t)1;
    }

    maxRow = need;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0; /* undefined, but return 0 for safety */

    /* Find row r such that r*(r+1)/2 >= n */
    idx_t r = 1;
    while (r * (r + 1) / 2 < n) ++r;

    ensure_row(r);

    /* Number of elements before this row */
    idx_t before = (r - 1) * r / 2;
    idx_t c = n - before; /* column within the row */

    return tri[r][c];
}
