/*
 * A211788  Triangle enumerating certain two-line arrays of positive integers.
 * https://oeis.org/A211788
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, tabl
 * Formula:   a(n) is defined by the recurrence for the triangle T(n,k) T(1,1)
 *            = 1; T(n,n) = T(n,n-1); T(n+1,k) = sum_{j=1..k} (2*k-2*j+1) *
 *            T(n,j) a(n) equals the entry T(m, k+1) where m = smallest
 *            integer with m*(m+1)/2 >= n k = n - (m*(m-1)/2 + 1)
 * Range:     u8 n <= 18, u16 n <= 42, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

seq_t a(idx_t n)
{
    /* static state for memoisation */
    static bool initialized = false;
    static seq_t **T = NULL;          /* triangular table T[1..m][1..m] */
    static size_t max_m = 0;          /* largest m computed */

    if (!initialized) {
        T = NULL;
        initialized = true;
    }

    /* find the row m such that m*(m+1)/2 >= n   (m starts at 1) */
    size_t m = 1;
    while ((idx_t)m * (m + 1) / 2 < n) {
        ++m;
    }

    /* enlarge the table if required */
    if (m > max_m) {
        /* allocate new table pointers */
        seq_t **newT = (seq_t **)malloc((m + 1) * sizeof(seq_t *));
        if (newT == NULL) {
            /* allocation failure – fall back to zero result */
            return (seq_t)0;
        }
        /* copy existing rows */
        if (T != NULL) {
            memcpy(newT, T, (max_m + 1) * sizeof(seq_t *));
            free(T);
        }
        /* initialise new rows to NULL */
        for (size_t i = max_m + 1; i <= m; ++i) newT[i] = NULL;
        T = newT;

        /* compute new rows of the triangle */
        for (size_t i = max_m + 1; i <= m; ++i) {
            /* allocate space for row i: indices 0..i (0 unused) */
            T[i] = (seq_t *)malloc((i + 2) * sizeof(seq_t));
            if (T[i] == NULL) return (seq_t)0; /* allocation failure */

            if (i == 1) {            /* base case */
                T[1][1] = 1;
            } else {
                T[i][1] = 1;          /* first entry is always 1 */

                /* compute T[i][k] for 2 <= k <= i-1 */
                for (size_t k = 2; k <= i - 1; ++k) {
                    seq_t sum = 0;
                    for (size_t j = 1; j <= k; ++j) {
                        seq_t coef = (seq_t)(2 * k - 2 * j + 1);
                        sum += coef * T[i - 1][j];
                    }
                    T[i][k] = sum;
                }
                /* last entry equals the previous one */
                T[i][i] = T[i][i - 1];
            }
        }
        max_m = m;
    }

    /* compute the offset of the first entry of row m */
    idx_t first = (idx_t)m * (m - 1) / 2 + 1;
    size_t k = (size_t)(n - first);           /* 0 <= k <= m-1 */

    return T[m][k + 1];
}
