/*
 * A060029  Expansion of (1-x-x^N)/((1-x)(1-x^2)(1-x^3)...(1-x^N)) for N = 10.
 * https://oeis.org/A060029
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = P_10(n) - P_10(n-1) - P_10(n-10) where P_N(m) is the
 *            number of partitions of m into parts not exceeding N. This
 *            follows from the generating function
 *            (1-x-x^10)/((1-x)(1-x^2)...(1-x^10))
 * Range:     i8 n <= 33, i16 n <= 58, i32 n <= 226, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

// If the underlying build uses arbitrary precision integers,
// the type seq_t will support all required arithmetic.
#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n) {
    /* 1. Maintain a static array 'dp' where dp[m] = number of partitions
       of m into parts <= 10.  Recompute the array only when a larger
       index is needed. */
    static seq_t *dp = NULL;
    static idx_t dp_size = 0;  // current maximum index stored (>= last computed)

    /* Ensure the dp array can provide values up to 'max_needed' */
    idx_t max_needed = n;
    if (max_needed < 9) max_needed = 9; /* to handle n-10 case without underflow */
    idx_t new_size = max_needed;  /* inclusive index 0..new_size */

    if (dp_size < new_size + 10) {
        /* Allocate new array big enough for new_size + 10 (to simplify padding) */
        idx_t alloc_size = new_size + 10 + 1;  // +1 for zero-based indexing
        seq_t *new_dp = (seq_t *)realloc(dp, (size_t)alloc_size * sizeof(seq_t));
        if (!new_dp) { /* allocation failure – return 0 as safe fallback */
            return 0;
        }
        dp = new_dp;
        /* recompute from scratch up to new_size + 10 */
        for (idx_t i = 0; i <= new_size + 10; ++i) dp[i] = 0;
        dp[0] = 1;
        for (idx_t part = 1; part <= 10; ++part) {
            for (idx_t s = part; s <= new_size + 10; ++s) {
                dp[s] += dp[s - part];
            }
        }
        dp_size = new_size + 10;
    }

    /* 2. Compute a(n) = dp[n] - dp[n-1] - dp[n-10] (treat negative indices as 0) */
    seq_t result = dp[n];
    if (n >= 1) result -= dp[n - 1];
    if (n >= 10) result -= dp[n - 10];
    return result;
}
