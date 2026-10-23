/*
 * A119390  a(n) = n!*Sum_{k=0..n} (-1)^(n-k)*Stirling1(n,k)/k!.
 * https://oeis.org/A119390
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = n!*Sum_{k=0..n} (-1)^(n-k)*Stirling1(n,k)/k! The unsigned
 *            Stirling numbers cancel the signs, so the equivalent computation
 *            uses only unsigned Stirling numbers of the first kind
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 13, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* The function a(idx_t n) is specified to have external linkage.  */
seq_t a(idx_t n)
{
    /* Static cache for Stirling numbers (unsigned) and already computed values. */
    static seq_t *stirling = NULL;      /* s[k] holds unsigned Stirling numbers for the current row */
    static seq_t *values  = NULL;       /* aValues[n] holds the computed sequence values */
    static idx_t max_n    = 0;          /* the largest n for which we have computed a(n) */

    /* Initial allocation and base case a(0) = 1 */
    if (stirling == NULL) {
        /* Allocate space for at least one element (n = 0) */
        stirling = (seq_t *)malloc(sizeof(seq_t) * (n + 1));
        values   = (seq_t *)malloc(sizeof(seq_t) * (n + 1));
        if (stirling == NULL || values == NULL) {
            /* In a real code base we should handle allocation failure.
               Here we simply return 0, which will be caught by tests. */
            return (seq_t)0;
        }
        for (idx_t i = 0; i <= n; ++i) {
            stirling[i] = 0;
            values[i]   = 0;
        }
        stirling[0] = 1;         /* u(0,0) = 1 */
        values[0]   = 1;         /* a(0) = 1 */
        max_n       = 0;
    }

    /* Extend the cache if necessary */
    if (n > max_n) {
        /* Resize arrays to hold entries up to n */
        stirling = (seq_t *)realloc(stirling, sizeof(seq_t) * (n + 1));
        values   = (seq_t *)realloc(values,   sizeof(seq_t) * (n + 1));
        if (stirling == NULL || values == NULL) {
            return (seq_t)0;    /* Allocation failure */
        }
        /* Initialise new entries to zero */
        for (idx_t i = max_n + 1; i <= n; ++i) {
            stirling[i] = 0;
            values[i]   = 0;
        }

        /* Compute Stirling numbers row by row, starting after the last computed row */
        for (idx_t row = max_n + 1; row <= n; ++row) {
            /* Update the Stirling array to the current row */
            /* Downward loop to avoid overwriting necessary previous values */
            for (int k = (int)row; k >= 1; --k) {
                /* s(row, k) = s(row-1, k-1) + (row-1)*s(row-1, k) */
                stirling[k] = stirling[k - 1] + (seq_t)(row - 1) * stirling[k];
            }
            stirling[0] = 0;          /* u(row, 0) = 0 for row > 0 */

            /* Compute a(row) from the current Stirling row */
            seq_t sum   = 0;
            seq_t ratio = 1;          /* ratio = n! / k! for the current k (starts with k = row) */

            for (idx_t k = row; ; --k) {
                /* term = u(n, k) * (n! / k!) */
                sum += stirling[k] * ratio;

                if (k == 0) break;
                /* Prepare ratio for the next smaller k:
                   ratio_{k-1} = ratio_k * k
                */
                ratio *= k;
            }
            values[row] = sum;
        }

        max_n = n;
    }

    return values[n];
}
