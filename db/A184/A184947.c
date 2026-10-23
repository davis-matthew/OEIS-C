/*
 * A184947  Expansion of e.g.f. exp(x)*(1-x)^(-x).
 * https://oeis.org/A184947
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 12, u64 n <= 20, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS: A184947
   Expanding the e.g.f. exp(x)*(1-x)^(-x).
   Recurrence derived from the differential equation:
      a(0) = 1
      a(m+1) = a(m) + Σ_{r=1}^{m} a(m-r)*(m)_r
                          * (1 + 1/r)
   where (m)_r = m*(m-1)*…*(m-r+1) is the falling factorial.
   This uses only integer arithmetic and runs in O(n²) time. */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Allocate array for all terms up to n */
    seq_t *terms = (seq_t *)malloc((n + 1) * sizeof(seq_t));
    if (!terms) return 0;  /* out of memory, return 0 */

    /* Initial term a(0) = 1 */
    terms[0] = 1;

    /* Compute terms iteratively */
    for (idx_t m = 0; m < n; ++m) {
        seq_t cur = terms[m];   /* a(m) term */
        seq_t fall = 1;         /* (m)_0 */

        /* Sum over r = 1 .. m */
        for (idx_t r = 1; r <= m; ++r) {
            /* Update falling factorial (m)_r */
            fall = fall * (m - r + 1);

            /* term = a(m-r) * (m)_r */
            seq_t t = terms[m - r] * fall;

            /* Add t   +  t/r  where (1 + 1/r) = (r+1)/r */
            cur = cur + t;
            cur = cur + (t / r);  /* exact division */
        }

        terms[m + 1] = cur;  /* store a(m+1) */
    }

    seq_t result = terms[n];
    free(terms);
    return result;
}
