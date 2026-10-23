/*
 * A011572  Stirling numbers of second kind S2(23,n).
 * https://oeis.org/A011572
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) = StirlingNumberSecondKind(23,n). Recurrence: S(0,0)=1; for
 *            r>0: S(r,k)=k*S(r-1,k)+S(r-1,k-1)
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 2, u64 n <= 23, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n) {
    /* For all requested n≥1 */
    if (n < 1) return 0;             /* undefined for n<1: return 0 */

    /* Compute once the 23rd row of Stirling numbers of the second kind */
    static seq_t cache[24];          /* cache[1..23] holds S(23,k) */
    static bool computed = false;    /* flag indicates whether cache is ready */

    if (!computed) {
        seq_t dp[24] = {0};
        dp[0] = 1;                  /* S(0,0)=1 */

        for (idx_t r = 1; r <= 23; ++r) {
            seq_t next[24] = {0};
            for (idx_t k = 1; k <= r; ++k) {
                /* next[k] = k * dp[k] + dp[k-1] */
                next[k] = dp[k] * ((seq_t)k) + dp[k - 1];
            }
            for (idx_t k = 0; k <= r; ++k) dp[k] = next[k];
        }

        /* store results for indices 1..23 */
        for (idx_t k = 1; k <= 23; ++k) cache[k] = dp[k];
        computed = true;
    }

    if (n <= 23) return cache[n];     /* valid terms */
    return 0;                         /* out of defined range */
}
