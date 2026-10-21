/*
 * A093996  G.f.: Product_{k>=2} (1 - x^{F_k}) where F_k are the Fibonacci
 *          numbers.
 * https://oeis.org/A093996
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign
 * Formula:   not stated
 * Range:     i8 n <= 2659, i16 n <= 9292, i32 n <= 9292, i64 n <= 9292,
 *            i128 n <= 9292, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = coefficient of x^n in ∏_{k≥2}(1 – x^{F_k}),
   i.e. number of partitions of n into distinct Fibonacci parts with a
   sign (-1)^{#parts}.  Computed by dynamic programming. */
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static array holding the DP values up to the current maximum */
    static seq_t *dp = NULL;
    static idx_t cur_max = 0;  /* largest n for which dp is valid */

    if (n + 1 > cur_max) {
        /* enlarge and recompute the table up to n */
        size_t new_len = (size_t)(n + 1);
        seq_t *tmp = (seq_t *)realloc(dp, new_len * sizeof(seq_t));
        if (!tmp) {   /* allocation failed – return 0 */
            return (seq_t)0;
        }
        dp = tmp;

        /* initialise dp[0] = 1, others = 0 */
        dp[0] = (seq_t)1;
        for (idx_t i = 1; i <= n; ++i)
            dp[i] = (seq_t)0;

        /* generate Fibonacci numbers starting with 1, 2, 3, 5, ... */
        seq_t a_fib = (seq_t)1;
        seq_t b_fib = (seq_t)2;
        while ((idx_t)a_fib <= n) {
            idx_t f = (idx_t)a_fib;          /* current Fibonacci number */

            /* update dp using this Fibonacci part */
            for (idx_t i = n; i >= f; --i)
                dp[i] -= dp[i - f];

            /* advance to next Fibonacci */
            seq_t next = a_fib + b_fib;
            a_fib = b_fib;
            b_fib = next;
        }

        cur_max = n;
    }

    return dp[n];
}
