/*
 * A219487  Number of representations of n as a sum of distinct elements of
 *          the generalized Fibonacci sequence beginning 4, 3, 7, 10, 17, 27,
 *          ....
 * https://oeis.org/A219487
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, look
 * Formula:   not stated
 * Range:     u8 n <= 8127, u16 n <= 8127, u32 n <= 8127, u64 n <= 8127,
 *            u128 n <= 8127, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Number of representations of n as a sum of distinct elements of the
    generalized Fibonacci sequence beginning with 4, 3, 7, 10, 17, 27, …
    Implementation uses standard subset‑sum dynamic programming.  */

#include <stddef.h>
#include <stdlib.h>

/* seq_t and idx_t are defined by the build system.  */
seq_t a(idx_t n)
{
    /* Special trivial case for n = 0.  The DP below also returns 1, but
       this guard keeps the function short.  */
    if (n == 0) {
        return 1;
    }

    /* Allocate an array large enough for all intermediary sums.  */
    size_t sz = (size_t)n + 1;
    seq_t *dp = (seq_t *)malloc(sz * sizeof(seq_t));
    if (!dp) {
        return 0;          /* allocation failed – return zero.  */
    }

    /* Initialise DP array: only the empty sum is initially achievable. */
    for (size_t i = 0; i < sz; ++i) {
        dp[i] = 0;
    }
    dp[0] = 1;

    /* Generate the sequence terms and update DP for each term.  */
    idx_t term1 = 4;  /* S(0) */
    idx_t term2 = 3;  /* S(1) */

    /* Process the first two distinct terms, if they are not larger than n. */
    if (term1 <= n) {
        for (idx_t s = n; s >= term1; --s) {
            dp[s] = dp[s] + dp[s - term1];
        }
    }
    if (term2 <= n) {
        for (idx_t s = n; s >= term2; --s) {
            dp[s] = dp[s] + dp[s - term2];
        }
    }

    /* Generate subsequent terms as the sum of the two previous ones.  */
    while (1) {
        idx_t next = term1 + term2;
        if (next > n || next < term1) {  /* overflow guard */
            break;
        }
        for (idx_t s = n; s >= next; --s) {
            dp[s] = dp[s] + dp[s - next];
        }
        term1 = term2;
        term2 = next;
    }

    /* The sought value is stored in the last element of the array.  */
    seq_t result = dp[n];

    /* Clean up and return.  */
    free(dp);
    return result;
}
