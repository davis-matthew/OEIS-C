/*
 * A170278  Number of reduced words of length n in Coxeter group on 29
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^41 = I.
 * https://oeis.org/A170278
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   the sequence satisfies the recurrence obtained from the *
 *            generating function N(t)/D(t) where * * N(t) = 1 + 2t + 2t^2 +
 *            ... + 2t^40 + t^41 * D(t) = 378 t^41 + 1 - 27(t + t^2 + ... +
 *            t^40) * * For 0 ≤ n ≤ 40: * a(n) = c_n + 27 * Σ_{i=0}^{n-1} a(i)
 *            (c_0 = 1, c_1..c_40 = 2) * For n = 41: * a(41) =...
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 13, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == 0ULL)
        return (seq_t)1;

    /* Allocate array for a[0..n] */
    size_t sz = (size_t)n + 1;
    seq_t *a = (seq_t *)malloc(sz * sizeof(seq_t));
    if (!a)
        return (seq_t)0; /* allocation failure */

    /* a[0] */
    a[0] = (seq_t)1;

    /* For n ≤ 40 we need prefix sums up to 40 */
    seq_t pref[42];
    pref[0] = a[0];

    idx_t i;
    /* Compute a[1] .. a[40] (or up to n) */
    for (i = 1; i <= (n > 40U ? 40U : n); ++i) {
        a[i] = (seq_t)2 + (seq_t)27 * pref[i - 1];
        pref[i] = pref[i - 1] + a[i];
    }

    /* Return if n ≤ 40 */
    if (n <= 40ULL) {
        seq_t res = a[n];
        free(a);
        return res;
    }

    /* Compute a[41] */
    seq_t sum_1_to_40 = pref[40] - a[0];                 /* Σ_{i=1}^{40} a(i) */
    a[41] = (seq_t)27 * sum_1_to_40;
    a[41] -= (seq_t)377;  /* a[41] = 27*(sum_1_to_40) - 377 */
    pref[41] = pref[40] + a[41];

    if (n == 41ULL) {
        seq_t res = a[41];
        free(a);
        return res;
    }

    /* Sliding window sum of the last 40 terms: a[2] .. a[41] for computing a[42] */
    seq_t sum_last40 = pref[41] - pref[1]; /* Σ_{i=2}^{41} a(i) */

    /* Compute a[42] .. a[n] */
    for (i = 42; i <= n; ++i) {
        a[i] = (seq_t)27 * sum_last40;
        a[i] -= (seq_t)378 * a[i - 41];
        /* Update sliding window: drop a[i-40], add a[i] */
        sum_last40 += a[i];
        sum_last40 -= a[i - 40];
    }

    seq_t res = a[n];
    free(a);
    return res;
}
