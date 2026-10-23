/*
 * A107443  Expansion of g.f.: (1+3*x^2)/((1-x)*(1+x+2*x^2)*(1-x+2*x^2)).
 * https://oeis.org/A107443
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign
 * Formula:   not stated
 * Range:     i8 n <= 13, i16 n <= 31, i32 n <= 63, i64 n <= 127,
 *            i128 n <= 255, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) satisfies the linear recurrence
 *   a(0)=1 , a(1)=1 , a(2)=1 , a(3)=1 , a(4)=-3
 *   a(n) = a(n-1) - 3*a(n-2) + 3*a(n-3) - 4*a(n-4) + 4*a(n-5)   (n ≥ 5)
 *
 * This recurrence is derived from the generating function
 *   G(x) = (1+3*x^2)/((1-x)*(1+x+2*x^2)*(1-x+2*x^2)).
 */
#include <stdlib.h>

seq_t a(idx_t n) {
    /* Static storage for memoization of computed terms */
    static seq_t *dp = NULL;      /* array of a(k) values */
    static size_t dp_cap = 0;     /* current allocated capacity */
    static size_t dp_size = 0;    /* number of terms initialized (dp[0..dp_size-1]) */

    /* Initial initialization on first call */
    if (dp == NULL) {
        dp_cap = 8;          /* start with small capacity */
        dp = (seq_t *)malloc(dp_cap * sizeof(seq_t));
        if (dp == NULL) return 0;   /* allocation failure, return 0 as fallback */

        /* initial 5 terms of the sequence */
        dp[0] = (seq_t)1;
        dp[1] = (seq_t)1;
        dp[2] = (seq_t)1;
        dp[3] = (seq_t)1;
        dp[4] = (seq_t)-3;
        dp_size = 5;
    }

    /* Ensure capacity for n */
    if (n >= dp_cap) {
        size_t new_cap = dp_cap;
        while (n >= new_cap)
            new_cap = new_cap ? new_cap * 2 : 8;
        seq_t *new_dp = (seq_t *)realloc(dp, new_cap * sizeof(seq_t));
        if (new_dp == NULL)
            return 0; /* allocation failure, fallback */
        dp = new_dp;
        dp_cap = new_cap;
    }

    /* Compute missing terms up to n using recurrence */
    while (dp_size <= n) {
        seq_t a1 = dp[dp_size - 1];
        seq_t a2 = dp[dp_size - 2];
        seq_t a3 = dp[dp_size - 3];
        seq_t a4 = dp[dp_size - 4];
        seq_t a5 = dp[dp_size - 5];
        dp[dp_size] = a1 - (seq_t)(3) * a2 + (seq_t)(3) * a3
                       - (seq_t)(4) * a4 + (seq_t)(4) * a5;
        dp_size++;
    }

    return dp[n];
}
