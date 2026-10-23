/*
 * A153738  Adjusted recursive triangular sequence with row sums 2*(n+5)!/6!:
 *          A(n,k)= A(n - 1, k - 1) + A(n - 1, k) + (n + 4)*(n + 3)*A(n - 2, k
 *          - 1).
 * https://oeis.org/A153738
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   A153738 – Adjusted recursive triangular sequence. * Recurrence
 *            for row r ≥ 4: * A(r,k) = A(r-1,k-1) + A(r-1,k) +
 *            (r+4)*(r+3)*A(r-2,k-1) * Initial rows (r = 1, 2, 3): * r=1: [2]
 *            * r=2: [7, 7] * r=3: [1, 110, 1] * The sequence a(n) is the
 *            flattening of the triangle: * a(1)=2, a(2)=7, a(3)=7, a(4)=1...
 * Range:     u8 n <= 7, u16 n <= 23, u32 n <= 47, u64 n <= 47, u128 n <= 47,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>
#ifdef __cplusplus
#  include <new>
#endif

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* not defined for offset 1, but safe */

    seq_t *prev1 = NULL;   /* row r-1 */
    seq_t *prev2 = NULL;   /* row r-2 */
    idx_t cumulative = 0;  /* total number of terms generated so far */

    idx_t r = 0;           /* current row number (1‑based) */

    while (cumulative < n) {
        r++;
        size_t len = (size_t)r;                /* length of current row */
        /* allocate array of len seq_t elements */
        seq_t *cur = (seq_t *)malloc((len + 1) * sizeof(seq_t));
        if (!cur) abort();                     /* unrecoverable */
#ifdef __cplusplus
        /* Construct elements in C++ builds */
        for (size_t i = 0; i < len; ++i) {
            new (&cur[i]) seq_t();
        }
#endif

        /* ------- fill current row ------- */
        if (r == 1) {
            cur[0] = (seq_t)2;
        } else if (r == 2) {
            cur[0] = (seq_t)7;
            cur[1] = (seq_t)7;
        } else if (r == 3) {
            cur[0] = (seq_t)1;
            cur[1] = (seq_t)110;
            cur[2] = (seq_t)1;
        } else {
            seq_t factor = ((seq_t)(r + 4)) * ((seq_t)(r + 3));
            cur[0]   = prev1[0];
            cur[len-1] = prev1[len-2];
            for (idx_t k = 1; k < r - 1; ++k) {
                cur[k] = prev1[k-1] + prev1[k] + factor * prev2[k-1];
            }
        }

        /* ------- check if desired term is inside this row ------- */
        idx_t new_cum = cumulative + (idx_t)len;
        if (new_cum >= n) {
            idx_t pos = n - cumulative - 1;      /* 0‑based index in row */
            seq_t result = cur[pos];

            /* tidy up allocated memory */
            free(prev2);
            free(prev1);
            free(cur);
            return result;
        }

        /* ------- prepare for next iteration ------- */
        free(prev2);
        prev2 = prev1;
        prev1 = cur;
        cumulative = new_cum;
    }

    /* unreachable but required for control‑flow */
    return (seq_t)0;
}
