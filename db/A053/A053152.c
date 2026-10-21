/*
 * A053152  Number of 2-element intersecting families whose union is an
 *          n-element set.
 * https://oeis.org/A053152
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (3^n - 2^n - 1) / 2 (OEIS A053152) Recurrence: a(n) =
 *            6*a(n-1) - 11*a(n-2) + 6*a(n-3) for n >= 4, with initial values
 *            a(1)=0, a(2)=2, a(3)=9
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 20, u64 n <= 41, u128 n <= 81,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation uses the recurrence for O(n) time with linear
   memoisation.  Only standard C11 facilities are required. */
#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *A = NULL;          /* memoised values, 1‑based */
    static idx_t max_n = 0;          /* largest n computed so far */

    /* initialise or extend the cache up to n */
    if (A == NULL) {
        /* allocate at least up to the first 3 terms */
        idx_t needed = n < 3 ? 3 : n;
        size_t sz = (size_t)(needed + 1);
        A = (seq_t *)malloc(sz * sizeof(seq_t));
        A[1] = 0;
        A[2] = 2;
        A[3] = 9;
        max_n = 3;

        /* compute any further terms up to n */
        if (n > 3) {
            for (idx_t i = 4; i <= n; ++i) {
                A[i] = 6 * A[i - 1] - 11 * A[i - 2] + 6 * A[i - 3];
            }
            max_n = n;
        }
    } else if (n > max_n) {
        /* grow the array to accommodate the new index */
        size_t new_sz = (size_t)(n + 1);
        A = (seq_t *)realloc(A, new_sz * sizeof(seq_t));

        /* compute the missing terms */
        for (idx_t i = max_n + 1; i <= n; ++i) {
            A[i] = 6 * A[i - 1] - 11 * A[i - 2] + 6 * A[i - 3];
        }
        max_n = n;
    }

    return A[n];
}
