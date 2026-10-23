/*
 * A101918  G.f. satisfies: A(x) = 1/(1 + x*A(x^8)) and also the continued
 *          fraction: 1 + x*A(x^9) = [1; 1/x, 1/x^8, 1/x^64, 1/x^512, ...,
 *          1/x^(8^(n-1)), ...].
 * https://oeis.org/A101918
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(0) = 1; a(n) = -Sum_{k=0..floor((n-1)/8)} a(k)*a(n-8*k-1)
 * Range:     i8 n <= 35, i16 n <= 70, i32 n <= 147, i64 n <= 287,
 *            i128 n <= 579, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* The function is called repeatedly with arbitrary n in any order.
   We keep a static cache (dp) that grows as necessary.  The array is
   allocated with malloc so that it can store arbitrary‑precision seq_t
   objects, and the memory is never freed again (avoiding destructor
   issues on C++ builds). */
seq_t a(idx_t n)
{
    /* static cache of computed terms */
    static seq_t *dp = NULL;        /* pointer to the array */
    static size_t dp_size = 0;      /* number of terms currently stored */
    static size_t dp_cap  = 0;      /* allocated capacity */

    /* initialise cache on first call */
    if (dp == NULL) {
        dp_cap  = 256;              /* start with a small capacity */
        dp      = (seq_t*)malloc(dp_cap * sizeof(seq_t));
        if (dp == NULL) abort();   /* can't continue without storage */
        dp[0] = 1;                 /* a(0) = 1 */
        dp_size = 1;
    }

    /* grow capacity if needed */
    if ((size_t)n + 1 > dp_cap) {
        size_t new_cap = dp_cap;
        while (new_cap <= (size_t)n) new_cap <<= 1;
        seq_t *newdp = (seq_t*)malloc(new_cap * sizeof(seq_t));
        if (newdp == NULL) abort();
        for (size_t i = 0; i < dp_size; ++i) newdp[i] = dp[i];
        free(dp);
        dp = newdp;
        dp_cap = new_cap;
    }

    /* compute missing terms up to n (inclusive) */
    for (size_t i = dp_size; i <= (size_t)n; ++i) {
        if (i == 0) {                /* a(0) already set */
            dp[0] = 1;
            continue;
        }
        seq_t sum = 0;
        /* floor((i-1)/8) is non‑negative for i>0 */
        idx_t maxk = (idx_t)((i - 1) / 8);
        for (idx_t k = 0; k <= maxk; ++k) {
            /* index n-8*k-1 is safe because k <= maxk */
            size_t idx2 = i - 8 * k - 1;
            sum -= dp[(size_t)k] * dp[idx2];
        }
        dp[i] = sum;
    }
    dp_size = (size_t)n + 1;   /* we now have terms 0..n */

    return dp[(size_t)n];
}
