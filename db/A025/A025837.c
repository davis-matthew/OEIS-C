/*
 * A025837  Expansion of 1/((1-x^3)*(1-x^5)*(1-x^9)).
 * https://oeis.org/A025837
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-3) + a(n-5) + a(n-9) - a(n-8) - a(n-12) - a(n-14) +
 *            a(n-17) with a(0)=1 and a(k<0)=0 (recurrence from
 *            (1-x³)(1-x⁵)(1-x⁹)=0)
 * Range:     u8 n <= 251, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

#ifndef __cplusplus
#  define CAST_PTR(Type, Expr) ((Type)(Expr))
#else
#  define CAST_PTR(Type, Expr) (reinterpret_cast<Type>(Expr))
#endif

/* Compute a(n).  Sequence is cached so subsequent calls are O(1). */
seq_t a(idx_t n)
{
    /* static cache of already computed values */
    static idx_t   max_cached = 0;   /* largest n that has been computed */
    static seq_t *dp         = NULL;/* array of cached values */
    static size_t dp_cap     = 0;    /* current capacity of the dp array */

    /* First call: allocate space for the requested n (at least 1)   */
    if (dp == NULL) {
        dp_cap     = (size_t)((n >= 1) ? n + 1 : 1);
        dp         = CAST_PTR(seq_t *, calloc(dp_cap, sizeof(seq_t)));
        if (dp == NULL) return 0;          /* out-of-memory: dummy 0 */
        dp[0]      = (seq_t)1;              /* a(0)=1 */
        max_cached = 0;
    }

    /* Grow the array if a larger n is requested */
    if ((size_t)n >= dp_cap) {
        size_t new_cap = (size_t)((n >= 1) ? n + 1 : 1);
        seq_t *new_dp = CAST_PTR(seq_t *, realloc(dp, new_cap * sizeof(seq_t)));
        if (new_dp == NULL) return 0;      /* out-of-memory */

        /* Zero the newly allocated portion */
        for (size_t i = dp_cap; i < new_cap; ++i) new_dp[i] = (seq_t)0;

        dp     = new_dp;
        dp_cap = new_cap;
    }

    /* Compute missing values up to n, incrementally */
    for (idx_t i = max_cached + 1; i <= n; ++i) {
        seq_t pos = (seq_t)0;
        seq_t neg = (seq_t)0;

        if (i >= 3)  pos += dp[(size_t)(i - 3)];
        if (i >= 5)  pos += dp[(size_t)(i - 5)];
        if (i >= 9)  pos += dp[(size_t)(i - 9)];
        if (i >= 17) pos += dp[(size_t)(i - 17)];

        if (i >= 8)  neg += dp[(size_t)(i - 8)];
        if (i >=12)  neg += dp[(size_t)(i -12)];
        if (i >=14)  neg += dp[(size_t)(i -14)];

        dp[(size_t)i] = pos - neg; /* pos >= neg by definition */
    }
    max_cached = n;

    return dp[(size_t)n];
}
