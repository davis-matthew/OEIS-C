/*
 * A170287  Number of reduced words of length n in Coxeter group on 38
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^41 = I.
 * https://oeis.org/A170287
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1 * a(n) = p(n) + 36 * Σ_{i=1}^{min(n,40)} a(n-i) -
 *            (n>=41 ? 666*a(n-41) : 0) * where p(n) = 1 (n=0), 2 (1<=n<=40),
 *            1 (n=41), 0 (n>=42) * This implements the linear recurrence
 *            derived from the rational generating function * for OEIS
 *            sequence A170287
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 12, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>    /* for size_t */
#include <stdint.h>    /* for uint64_t if needed */

seq_t a(idx_t n) {
    /* static buffer, large enough for typical usage (up to ~10000) */
    #define MAX_N 20000
    static seq_t memo[MAX_N + 1];
    static idx_t computed = 0;   /* number of terms stored (next free index) */

    /* ensure we have enough space */
    if (n > MAX_N) {
        /* fallback: compute on the fly up to n, ignoring memoization */
        seq_t last[41] = {0};
        seq_t val, sum, term;
        seq_t p;
        if (n == 0) return (seq_t)1;

        /* compute initial 41 values */
        for (idx_t i = 0; i <= 41 && i <= n; ++i) {
            /* compute p(i) */
            if (i == 0) {
                p = (seq_t)1;
            } else if (i <= 40) {
                p = (seq_t)2;
            } else if (i == 41) {
                p = (seq_t)1;
            } else {
                p = (seq_t)0;
            }

            /* sum of previous up to 40 terms */
            sum = (seq_t)0;
            for (idx_t j = 1; j <= i && j <= 40; ++j) {
                sum += memo[i - j];
            }

            term = (seq_t)36 * sum;
            if (i >= 41) {
                term -= (seq_t)666 * memo[i - 41];
            }
            val = p + term;

            memo[i] = val;
        }
        return memo[n];
    }

    /* if value already computed, return it */
    if (n < computed) {
        return memo[n];
    }

    /* compute from current size up to n */
    for (idx_t i = computed; i <= n; ++i) {
        seq_t p;
        if (i == 0) {
            p = (seq_t)1;
        } else if (i <= 40) {
            p = (seq_t)2;
        } else if (i == 41) {
            p = (seq_t)1;
        } else {
            p = (seq_t)0;
        }

        seq_t sum = (seq_t)0;
        idx_t limit = (i < 40) ? i : (idx_t)40;
        for (idx_t j = 1; j <= limit; ++j) {
            sum += memo[i - j];
        }

        seq_t term = (seq_t)36 * sum;
        if (i >= 41) {
            term -= (seq_t)666 * memo[i - 41];
        }
        memo[i] = p + term;
    }

    computed = n + 1;
    return memo[n];
}
