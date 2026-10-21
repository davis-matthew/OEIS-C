/*
 * A098666  Triangle read by rows, 1<=k<=n: the n-th row contains the first n
 *          numbers after pairwise reducing all common divisors from left to
 *          right.
 * https://oeis.org/A098666
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 105, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A098666:
   Triangle read by rows.  For n ≥ 1 compute the row r with r(r+1)/2 ≥ n
   and the column k = n − r(r−1)/2.  The triangle entry T(r,k) is defined by

        T(1,1) = 1
        X(n,0) = n
        X(n,k) = X(n,k−1) / gcd( T(n−1,k), X(n,k−1) )    (1 ≤ k < n)
        T(n,k) = T(n−1,k) / gcd( T(n−1,k), X(n,k−1) )    (1 ≤ k < n)
        T(n,n) = X(n,n−1)

   The sequence value a(n) is exactly T(r,k) for the indices found above. */

#include <stdlib.h>
#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;           /* defined for completeness */

    /* Find the row r with r(r+1)/2 ≥ n */
    idx_t low = 1, high = n;
    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        __int128 sum = (__int128)mid * (mid + 1) / 2;
        if (sum >= n)
            high = mid;
        else
            low = mid + 1;
    }
    idx_t r = low;

    /* Column index k (1‑based) */
    __int128 prev_sum = (__int128)r * (r - 1) / 2;
    idx_t k = n - (idx_t)prev_sum;

    /* Allocate temporary rows and X array */
    seq_t *prev = (seq_t *)malloc(sizeof(seq_t) * (r + 1));
    seq_t *cur  = (seq_t *)malloc(sizeof(seq_t) * (r + 1));
    seq_t *X    = (seq_t *)malloc(sizeof(seq_t) * r);
    if (!prev || !cur || !X) {
        free(prev); free(cur); free(X);
        return 0;
    }

    /* Row 1 */
    prev[1] = (seq_t)1;

    /* Build rows 2..r */
    for (idx_t i = 2; i <= r; ++i) {
        X[0] = (seq_t)i;
        for (idx_t j = 1; j < i; ++j) {
            seq_t g = gcd(prev[j], X[j - 1]);
            cur[j] = prev[j] / g;
            X[j]   = X[j - 1] / g;
        }
        cur[i] = X[i - 1];
        /* swap prev and cur */
        seq_t *tmp = prev;
        prev = cur;
        cur = tmp;
    }

    seq_t result = prev[k];
    free(prev);
    free(cur);
    free(X);
    return result;
}
