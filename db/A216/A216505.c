/*
 * A216505  Number of values of k for which n can be written in the form
 *          a^2+k*b^2, a > 0, b > 0, k >= 0.
 * https://oeis.org/A216505
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   The sequence A216505 counts the number of distinct integers k (k ≥ 0) such that
   n = a² + k·b² for some integers a > 0, b > 0.  For each n ≥ 1 the algorithm
   iterates over all possible a and b, records the resulting k and finally
   returns the number of distinct k's found.
   An integer square root routine is implemented locally to avoid external
   dependencies.
*/

#include <stdlib.h>   /* malloc, free */

/* Integer square root: floor(sqrt(n))  (unsigned 64‑bit) */
static inline idx_t isqrt_u64(idx_t n) {
    idx_t low = 0, high = n;
    while (low <= high) {
        idx_t mid = (low + high) >> 1;
        idx_t mid2 = mid * mid;
        if (mid2 <= n) {
            low = mid + 1;
        } else {
            high = mid - 1;
        }
    }
    return high;
}

/* Main sequence function */
seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;   /* out of range for the sequence */

    /* Allocate a small boolean array to mark the distinct k values */
    uint8_t *seen = (uint8_t *)malloc((size_t)(n + 1));
    if (!seen) return (seq_t)0;     /* allocation failure */
    for (idx_t i = 0; i <= n; ++i) seen[i] = 0;

    idx_t max_a = isqrt_u64(n);    /* largest a with a² ≤ n */

    for (idx_t a_val = 1; a_val <= max_a; ++a_val) {
        idx_t a2   = a_val * a_val;
        idx_t d    = n - a2;           /* n - a² */
        if (d == 0) {                  /* k = 0 occurs for any b */
            seen[0] = 1;
            continue;
        }
        idx_t max_b = isqrt_u64(d);    /* largest b with b² ≤ d */
        for (idx_t b_val = 1; b_val <= max_b; ++b_val) {
            idx_t b2 = b_val * b_val;
            if (d % b2 == 0) {
                idx_t k = d / b2;      /* k = (n - a²) / b² */
                /* k ≤ d < n, so it fits in the array bounds */
                seen[k] = 1;
            }
        }
    }

    /* Count distinct k values */
    seq_t count = 0;
    for (idx_t i = 0; i <= n; ++i) {
        if (seen[i]) ++count;
    }

    free(seen);
    return count;
}
