/*
 * A161098  Number of partitions of n into squares where every part appears at
 *          least 10 times.
 * https://oeis.org/A161098
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   G.f.: ∏_{k≥1} (1 + x^(10*k^2)/(1-x^(k^2)))
 * Range:     u8 n <= 242, u16 n <= 747, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Number of partitions of n into squares where every part appears at least 10 times. */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Offset 1 – a(1) is first listed term. */
    if (n == 0) return 0;               /* undefined for n == 0 */

    idx_t N = n;
    /* dp[s] = number of partitions of sum s using processed squares. */
    seq_t *dp = (seq_t*)calloc((size_t)N + 1, sizeof(seq_t));
    if (!dp) return 0;
    dp[0] = 1;                           /* one way to form 0 */

    /* Temporary array to accumulate contributions of the current square. */
    seq_t *add = (seq_t*)calloc((size_t)N + 1, sizeof(seq_t));
    if (!add) { free(dp); return 0; }

    for (idx_t k = 1; ; ++k) {
        idx_t t = k * k;                  /* square value */
        if (t > N) break;                 /* no larger squares fit */

        idx_t base = 10 * t;              /* mandatory 10 copies of this square */
        if (base > N) continue;           /* cannot use this square at all */

        /* 1. clear add array */
        for (idx_t i = 0; i <= N; ++i) add[i] = 0;

        /* 2. add the mandatory 10 copies: for each existing partition of sum s,
           create a new partition of sum s + base. */
        for (idx_t s = 0; s + base <= N; ++s) {
            if (dp[s] != 0)
                add[s + base] += dp[s];
        }

        /* 3. propagate unlimited extra copies of t (unbounded coin). */
        for (idx_t s = t; s <= N; ++s) {
            add[s] += add[s - t];
        }

        /* 4. merge contributions back into dp. */
        for (idx_t s = 0; s <= N; ++s) {
            dp[s] += add[s];
        }
    }

    seq_t result = dp[N];
    free(add);
    free(dp);
    return result;
}
