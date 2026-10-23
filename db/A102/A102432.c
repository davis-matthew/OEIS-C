/*
 * A102432  Number of distinct polynomials of degree < n and coefficients > 0
 *          with solutions k > 1 such that p(k) = n.
 * https://oeis.org/A102432
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = A102431(n) - n + 2
 * Range:     u8 n <= 30, u16 n <= 155, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A102431(n) = sum_{b=2}^{n} R(b)
   where R(b) is the number of ways to write n as a sum of
   non‑negative multiples of the powers of b (i.e. ways to solve
   0 ≤ a_i,  n = Σ a_i b^i). For b > sqrt(n) this reduces to
   floor(n/b)+1.  For b ≤ sqrt(n) we compute R(b) by dynamic
   programming on the coin denominations {1, b, b^2, …}.  The
   constant polynomial contributes 1 for every base, so the final
   formula is a(n) = sum_{b=2}^{n} R(b) - n + 2. */

#include <stdlib.h>
#include <math.h>

seq_t a(idx_t n) {
    if (n == 0) return 0;            /* not required by problem, but safe */

    seq_t sumR = 0;
    idx_t sqrt_n = (idx_t)floor(sqrt((long double)n));

    /* Preallocate a DP array once; its size fits n+1. */
    seq_t *ways = (seq_t *)malloc((n + 1) * sizeof(seq_t));
    if (!ways) return 0;             /* in case of allocation failure */

    /* Bases up to sqrt(n) – use DP. */
    for (idx_t b = 2; b <= sqrt_n; ++b) {
        /* Reset DP array. */
        ways[0] = 1;
        for (idx_t i = 1; i <= n; ++i) ways[i] = 0;

        /* Compute powers of b up to n. */
        idx_t powers[64];     /* sufficient for n <= 10^9, larger n handled by loop */
        int pc = 0;
        idx_t p = 1;
        while (1) {
            powers[pc++] = p;
            if (p > n / b) break;   /* avoid overflow */
            p *= b;
        }

        /* DP over coin denominations = powers of b. */
        for (int idx = 0; idx < pc; ++idx) {
            idx_t coin = powers[idx];
            for (idx_t amt = coin; amt <= n; ++amt) {
                ways[amt] += ways[amt - coin];
            }
        }

        sumR += ways[n];
    }

    /* Bases > sqrt(n) – only powers 1 and b.  R(b) = floor(n/b) + 1. */
    for (idx_t b = sqrt_n + 1; b <= n; ++b) {
        sumR += (seq_t)(n / b) + (seq_t)1;
    }

    free(ways);

    /* Final result: a(n) = sumR - n + 2. */
    seq_t result = sumR - (seq_t)n + (seq_t)2;
    return result;
}
