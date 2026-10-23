/*
 * A108758  a(n) = 2*a(n-1) - a(n-4) + a(n-5) with a(-1)=a(0)=a(1)=1, a(2)=2,
 *          a(3)=4, a(4)=7.
 * https://oeis.org/A108758
 *
 * Offset:    -1    (a(-1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*a(n-1) - a(n-4) + a(n-5); Initial values: a(-1)=1,
 *            a(0)=1, a(1)=1, a(2)=2, a(3)=4, a(4)=7 Recurrence holds for all
 *            n ≥ 5
 * Range:     u8 n <= 9, u16 n <= 17, u32 n <= 33, u64 n <= 33, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>   /* for malloc, realloc */

seq_t a(idx_t n)
{
    /* Pre‑defined terms for n = -1 .. 4 */
    static const seq_t init[6] = {1,1,1,2,4,7};

    /* Memoisation table:
       dp[i] = a(i-1)   for i ≥ 0   (dp[0] = a(-1))          */
    static seq_t *dp      = NULL;   /* dynamically growable array   */
    static size_t cap     = 0;      /* allocated capacity (in entries) */
    static idx_t  last    = -2;     /* largest n already computed   */

    /* Helper: ensure that dp has capacity for at least 'need' entries   */
    if (cap < (size_t)(n + 2)) {
        size_t new_cap = cap == 0 ? 16 : cap * 2;
        if (new_cap < (size_t)(n + 2))
            new_cap = (size_t)(n + 2);
        seq_t *tmp = (seq_t*)realloc(dp, new_cap * sizeof(seq_t));
        if (!tmp) return 0;          /* allocation failure -> zero */
        dp  = tmp;
        cap = new_cap;
    }

    /* Initialise first six entries if necessary */
    if (last < 4) {
        for (size_t i = 0; i < 6 && i < cap; ++i)
            dp[i] = init[i];
        last = 4;
    }

    /* Answer stored values directly for n ≤ 4 */
    if (n <= 4)
        return init[n + 1];      /* index shift: a(-1) -> init[0] */

    /* Compute forward up to requested n (only if not already done) */
    for (idx_t i = last + 1; i <= n; ++i) {
        /* dp[i+1] = a(i) = 2*a(i-1) - a(i-4) + a(i-5) */
        dp[i + 1] = 2 * dp[i] - dp[i - 3] + dp[i - 4];
    }
    last = n;

    return dp[n + 1];
}
