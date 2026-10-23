/*
 * A193788  Triangular array: the fusion of polynomial sequences P and Q given
 *          by p(n,x)=(x+1)^n and q(n,x)=1+x^n.
 * https://oeis.org/A193788
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = entry in the triangular array obtained by fusing the
 *            polynomial sequences p(n,x) = (x+1)^n and q(n,x) = 1 + x^n. The
 *            array is built row by row using the recurrences: T(0,0)=1
 *            T(1,0)=T(1,1)=1 T(r,0)=2·T(r-1,0) T(r,r)=3^(r-1) T(r,r-1)=1
 *            T(r,c)=T(r-1,c-1)+2·T(r-1,c) (1 ≤ c ≤ r-2)
 * Range:     u8 n <= 34, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

/* The function must be visible to the linker; external linkage is
   guaranteed by omitting 'static'. */
seq_t a(idx_t n)
{
    /* Find the row r such that offset(r) ≤ n < offset(r+1),
       where offset(r) = r*(r+1)/2. */
    idx_t r = 0;
    while ((r + 1) * (r + 2) / 2 <= n) {
        ++r;
    }
    idx_t offset = r * (r + 1) / 2;
    idx_t col = n - offset;

    /* Row 0 is a single entry 1. */
    if (r == 0) {
        return (seq_t)1;
    }

    /* Allocate the first row (row 0). */
    seq_t *prev = (seq_t *)malloc(sizeof(seq_t));
    prev[0] = (seq_t)1;

    for (idx_t i = 1; i <= r; ++i) {
        size_t len = (size_t)(i + 1);           /* length of current row */
        seq_t *curr = (seq_t *)malloc(sizeof(seq_t) * len);

        if (i == 1) {                          /* row 1: [1, 1] */
            curr[0] = (seq_t)1;
            curr[1] = (seq_t)1;
        } else {
            /* Left edge. */
            curr[0] = prev[0] * (seq_t)2;

            /* Interior entries. */
            for (idx_t c = 1; c <= i - 2; ++c) {
                curr[c] = prev[c - 1] + prev[c] * (seq_t)2;
            }

            /* Just before the right edge. */
            curr[i - 1] = (seq_t)1;

            /* Right edge: 3^(i-1). */
            seq_t pow3 = (seq_t)1;
            for (idx_t k = 0; k < i - 1; ++k) {
                pow3 = pow3 * (seq_t)3;
            }
            curr[i] = pow3;
        }

        free(prev);
        prev = curr;
    }

    seq_t result = prev[col];
    free(prev);
    return result;
}
