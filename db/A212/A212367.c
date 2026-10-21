/*
 * A212367  Number of Dyck n-paths all of whose ascents and descents have
 *          lengths equal to 1 (mod 8).
 * https://oeis.org/A212367
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + Sum_{k=1..n-8} a(k)*a(n-8-k) if n>0; a(0) = 1
 * Range:     u8 n <= 21, u16 n <= 36, u32 n <= 64, u64 n <= 119,
 *            u128 n <= 228, big any n
 * Helpers:   seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>              /* size_t */
#include "../utils/seq_realloc.h"/* seq_realloc() helper */


seq_t a(idx_t n)
{
    /* Memoised table of values */
    static seq_t *A      = NULL;   /* array of a(i) */
    static size_t  cap   = 0;       /* allocated capacity */
    static idx_t   max   = 0;       /* indices up to max-1 are already computed */

    /* Grow the table if needed */
    if ((size_t)(n + 1) > cap) {
        size_t new_cap = n + 1;
        if (new_cap < cap * 2) new_cap = cap * 2;   /* geometric growth */
        seq_t *tmp = seq_realloc(A, cap, new_cap); /* safe for any seq_t */
        if (!tmp) return 0;                        /* allocation failure */
        /* zero the newly added elements */
        for (size_t i = cap; i < new_cap; ++i) tmp[i] = 0;
        A = tmp;  cap = new_cap;
    }

    /* Initialise base case if first use */
    if (max == 0) {
        A[0] = 1;
        max = 1;
    }

    /* Compute values up to n */
    for (idx_t i = max; i <= n; ++i) {
        if (i <= (idx_t)7) {
            A[i] = 1;                          /* a(0)…a(7) are 1 */
        } else {
            seq_t sum = A[i - 1];              /* a(n-1) term */
            for (idx_t k = 1; k <= i - 8; ++k) {
                sum += A[k] * A[i - 8 - k];    /* convolution part */
            }
            A[i] = sum;
        }
    }
    max = n + 1;                                 /* everything up to n ready */

    return A[n];
}
