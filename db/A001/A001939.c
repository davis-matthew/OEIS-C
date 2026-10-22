/*
 * A001939  Expansion of (psi(-x) / phi(-x))^5 in powers of x where phi(),
 *          psi() are Ramanujan theta functions.
 * https://oeis.org/A001939
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   G.f.: Product_{k>0, 4 does not divide k} 1/(1 - x^k)^5 (Euler
 *            transform of period-4 sequence [5,5,5,0]): a(0) = 1, n*a(n) =
 *            Sum_{k=1..n} c(k)*a(n-k), where c(k) = Sum_{d|k, 4 does not
 *            divide d} 5*d. Fixed-width builds instead multiply out the
 *            product directly (exact modulo 2^w)
 * Range:     u8 n <= 4, u16 n <= 11, u32 n <= 33, u64 n <= 109, u128 n <= 384,
 *            big any n
 * Helpers:   a001028_z, a001939_is_fixed
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>
#include "../utils/a001028_z.h"
#include "../utils/a001939_is_fixed.h"

static seq_t *a001939_A = NULL;
static idx_t a001939_have = 0;
static idx_t a001939_cap = 0;
static idx_t *a001939_C = NULL;

seq_t a(idx_t n) {
    if (a001939_is_fixed()) {
        seq_t *dp = a001028_z(n + 1);
        seq_t r;
        idx_t k, j;
        int t;
        dp[0] = 1;
        for (k = 1; k <= n; k++) {
            if (k % 4 == 0) continue;
            for (t = 0; t < 5; t++)
                for (j = k; j <= n; j++)
                    dp[j] += dp[j - k];
        }
        r = dp[n];
        free(dp);
        return r;
    }

    if (n >= a001939_have) {
        if (n >= a001939_cap) {
            idx_t newcap = 2 * a001939_cap;
            idx_t i, d, m;
            seq_t *na;
            if (newcap < n + 1) newcap = n + 1;
            na = a001028_z(newcap);
            for (i = 0; i < a001939_have; i++) na[i] = a001939_A[i];
            /* old array intentionally not freed (element destructors unknown) */
            a001939_A = na;
            free(a001939_C);
            a001939_C = (idx_t *)calloc((size_t)newcap, sizeof(idx_t));
            for (d = 1; d < newcap; d++) {
                if (d % 4 == 0) continue;
                for (m = d; m < newcap; m += d) a001939_C[m] += 5 * d;
            }
            a001939_cap = newcap;
        }
        {
            idx_t i, k;
            for (i = a001939_have; i <= n; i++) {
                if (i == 0) {
                    a001939_A[0] = 1;
                    continue;
                }
                seq_t sum = 0;
                for (k = 1; k <= i; k++)
                    sum += a001939_A[i - k] * (seq_t)a001939_C[k];
                a001939_A[i] = sum / (seq_t)i;
            }
            a001939_have = n + 1;
        }
    }
    return a001939_A[n];
}
