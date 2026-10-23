/*
 * A108629  Expansion of g.f.: (1+x - sqrt(1-2*x-3*x^2-4*x^3))/(2+2*x+2*x^2).
 * https://oeis.org/A108629
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 12, u32 n <= 21, u64 n <= 28, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A108629
   Recurrence: for n≥6,
        n*a(n) = (n-3)*a(n-1)+4*(n-3)*a(n-2)+3*(3n-10)*a(n-3)+(7n-27)*a(n-4)+2*(2n-9)*a(n-5)
   Initial values: a(1)=1, a(2)=0, a(3)=1, a(4)=2, a(5)=4
   (a(0) is undefined) */

#include <stdlib.h>

seq_t a(idx_t n) {
    if (n < 1) return 0;               /* undefined for n<1 */

    /* static memoisation table, 1‑based: dp[1] = a(1) */
    static seq_t *dp = NULL;
    static idx_t cap = 0;          /* allocated capacity (>= needed index) */
    static idx_t stored = 0;       /* highest index already computed (>=5 after init) */

    /* ensure capacity */
    if (cap <= n) {
        idx_t newcap = cap ? cap : 8;              /* start with small size */
        while (newcap <= n) newcap <<= 1;
        dp = (seq_t *)realloc(dp, (newcap + 1) * sizeof(seq_t));
        if (!dp) exit(EXIT_FAILURE);
        cap = newcap;
    }

    /* initialise base cases once */
    if (stored == 0) {
        dp[1] = (seq_t)1;     /* a(1) */
        dp[2] = (seq_t)0;     /* a(2) */
        dp[3] = (seq_t)1;     /* a(3) */
        dp[4] = (seq_t)2;     /* a(4) */
        dp[5] = (seq_t)4;     /* a(5) */
        stored = 5;
    }

    /* compute further terms if needed */
    if (n > stored) {
        for (idx_t i = stored + 1; i <= n; ++i) {
            /* use idx_t terms cast to seq_t for multipliers */
            seq_t term1 = (seq_t)(i - 3) * dp[i - 1];
            seq_t term2 = (seq_t)4 * (seq_t)(i - 3) * dp[i - 2];
            seq_t term3 = (seq_t)3 * (seq_t)(3 * i - 10) * dp[i - 3];
            seq_t term4 = (seq_t)(7 * i - 27) * dp[i - 4];
            seq_t term5 = (seq_t)2 * (seq_t)(2 * i - 9) * dp[i - 5];

            seq_t numer = term1 + term2 + term3 + term4 + term5;
            dp[i] = numer / (seq_t)i;           /* division is exact */
        }
        stored = n;
    }

    return dp[n];
}
