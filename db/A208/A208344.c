/*
 * A208344  Triangle of coefficients of polynomials u(n,x) jointly generated
 *          with A208345; see the Formula section.
 * https://oeis.org/A208344
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   T(n,k) = T(n-1,k) + 2*T(n-1,k-1) - 2*T(n-2,k-1) + T(n-2,k-2)
 *            with T(0,0)=1, T(1,0)=T(1,1)=1, T(n,k)=0 for k<0 or k>n. The
 *            sequence a(n) is the flattened triangle (row by row). This
 *            function returns a(n) for 1‑based n
 * Range:     u8 n <= 42, u16 n <= 69, u32 n <= 69, u64 n <= 69, u128 n <= 69,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* static cache for the triangle */
static seq_t  *triangle  = NULL;   /* flat array holding all rows */
static size_t  triangle_rows = 0;  /* largest row index computed (0‑based) */
static size_t  triangle_cap  = 0;  /* allocated capacity in seq_t elements */

seq_t a(idx_t n)
{
    if (n < 1) return 0;        /* the sequence is defined for n>=1 */

    /* convert n to zero‑based index m = n-1 */
    idx_t m = n - 1;

    /* solve for the row r: r*(r+1)/2 <= m < (r+1)*(r+2)/2       */
    idx_t r = (idx_t)((((double) (1 + 8 * (long double) m)) - 1.0) * 0.5);

    /* compute offset within the triangle for the element */
    idx_t offset   = r * (r + 1) / 2;          /* start index of row r */
    idx_t k        = m - offset;              /* column within the row (0‑based) */

    /* add rows up to r if not already present */
    if (r >= triangle_rows) {
        /* required capacity for rows 0…r: (r+1)(r+2)/2 elements */
        size_t new_cap = (size_t)((r + 1ULL) * (r + 2ULL) / 2ULL);
        if (triangle_cap < new_cap) {
            seq_t *tmp = (seq_t *)realloc(triangle, new_cap * sizeof(seq_t));
            if (!tmp) abort();      /* memory exhausted */
            triangle = tmp;
            triangle_cap = new_cap;
        }

        for (size_t i = triangle_rows; i <= (size_t)r; ++i) {
            idx_t cur_off = i * (i + 1) / 2;          /* start index of row i */

            if (i == 0) {                /* T(0,0) = 1 */
                triangle[cur_off] = (seq_t)1;
            } else if (i == 1) {          /* T(1,0)=T(1,1)=1 */
                triangle[cur_off]     = (seq_t)1;
                triangle[cur_off + 1] = (seq_t)1;
            } else {                      /* recurrence for i >= 2 */
                idx_t off_prev  = (i - 1) * i / 2;          /* row i-1 */
                idx_t off_prev2 = (i - 2) * (i - 1) / 2;    /* row i-2 */

                for (idx_t j = 0; j <= i; ++j) {
                    seq_t t1 = (j <= i - 1) ? triangle[off_prev + j] : (seq_t)0;
                    seq_t t2 = (j >= 1)       ? triangle[off_prev + j - 1] : (seq_t)0;
                    seq_t t3 = (j >= 1 && j <= i - 1)
                                  ? triangle[off_prev2 + j - 1]
                                  : (seq_t)0;
                    seq_t t4 = (j >= 2)       ? triangle[off_prev2 + j - 2] : (seq_t)0;

                    seq_t val = t1 + 2*t2 + t4 - 2*t3;
                    triangle[cur_off + j] = val;
                }
            }
        }
        triangle_rows = r;
    }

    /* return the desired element */
    return triangle[offset + k];
}
