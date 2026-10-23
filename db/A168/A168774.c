/*
 * A168774  Number of reduced words of length n in Coxeter group on 49
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^18 = I.
 * https://oeis.org/A168774
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 11, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: Sequence a(n) is given by the rational generating function
 *           G(t) = (t^18 + 2*t^17 + … + 2*t + 1) /
 *                 (1128*t^18 - 47*t^17 - … - 47*t + 1)
 *
 * Recurrence:
 *   For n = 0:  a(0) = 1
 *   For 1 ≤ n ≤ 17:
 *     a(n) = 2 + 47 * Σ_{k=0}^{n-1} a(k)
 *   For n = 18:
 *     a(18) = 1 + 47 * Σ_{k=1}^{17} a(k) – 1128 * a(0)
 *   For n ≥ 19:
 *     a(n) = 47 * Σ_{k=n-17}^{n-1} a(k) – 1128 * a(n-18)
 */
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Allocate array for n+1 terms */
    size_t size = (size_t)n + 1;
    seq_t *v = (seq_t *)malloc(size * sizeof(seq_t));
    if (!v) return (seq_t)0;  /* out of memory – unlikely in tests */

    /* a(0) */
    v[0] = (seq_t)1;

    /* Handle n==0 directly */
    if (n == 0) {
        seq_t res = v[0];
        free(v);
        return res;
    }

    /* Prefix sum of all computed terms so far */
    seq_t prefix = v[0];

    /* Compute a(1) … a(17) */
    for (idx_t i = 1; i <= 17 && i <= n; ++i) {
        v[i] = (seq_t)2 + (seq_t)47 * prefix;
        prefix += v[i];
    }

    if (n <= 17) {
        seq_t res = v[n];
        free(v);
        return res;
    }

    /* Compute a(18) */
    seq_t sum_1_to_17 = prefix - v[0];                 /* Σ a[1..17] */
    v[18] = (seq_t)1 + (seq_t)47 * sum_1_to_17 - (seq_t)1128 * v[0];
    prefix += v[18];

    if (n == 18) {
        seq_t res = v[18];
        free(v);
        return res;
    }

    /* For n ≥ 19: use sliding window of the last 17 terms */
    seq_t window = (seq_t)0;
    for (idx_t j = 2; j <= 18; ++j)  /* sum a[2..18] */
        window += v[j];

    for (idx_t i = 19; i <= n; ++i) {
        seq_t val = (seq_t)47 * window - (seq_t)1128 * v[i - 18];
        v[i] = val;
        prefix += val;

        /* Update the sliding window to the next 17-term block:
         * window = sum a[i-16 .. i] = old_window - a[i-17] + val
         */
        window += val;
        window -= v[i - 17];
    }

    seq_t ans = v[n];
    free(v);
    return ans;
}
