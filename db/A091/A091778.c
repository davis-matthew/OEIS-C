/*
 * A091778  Expansion of g.f. Product((1+x^i)/(1-x^i),i=1..n-1)/(1-x^n), with
 *          n = 7.
 * https://oeis.org/A091778
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = coefficient of x^n in * \prod_{i=1}^6 (1+x^i)/(1-x^i)
 *            \;\; / (1-x^7) * The coefficient is also a(n) =
 *            \sum_{k=0}^{⌊n/7⌋} b(n-7k), * where b(m) is the coefficient of
 *            x^m in * \prod_{i=1}^6 (1+x^i)/(1-x^i). * b(m) is computed by
 *            dynamic programming using the identity * (1+x^i)/(1-x^i) = 1
 *            +...
 * Range:     u8 n <= 10, u16 n <= 35, u32 n <= 43, u64 n <= 43, u128 n <= 43,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n) {
    /* allocate working arrays of length n+1 */
    idx_t N = n;
    seq_t *dp      = (seq_t*)malloc((N + 1) * sizeof(seq_t));
    seq_t *dp_next = (seq_t*)malloc((N + 1) * sizeof(seq_t));
    seq_t *s       = (seq_t*)malloc((N + 1) * sizeof(seq_t));
    seq_t *a_val   = (seq_t*)malloc((N + 1) * sizeof(seq_t));
    if (!dp || !dp_next || !s || !a_val) {
        /* allocation failed – leave for caller to handle */
        return (seq_t)0;
    }

    /* initial polynomial: product of zero factors = 1 */
    for (idx_t i = 0; i <= N; ++i) dp[i] = 0;
    dp[0] = 1;

    /* multiply successively by (1+x^i)/(1-x^i) for i = 1..6 */
    for (idx_t part = 1; part <= 6; ++part) {
        /* s[k] = sum_{t≥1} dp[k - t*part]  (with k≥t*part) */
        for (idx_t k = 0; k <= N; ++k) {
            if (k >= part) {
                /* s[k] = s[k-part] + dp[k-part] */
                s[k] = s[k - part] + dp[k - part];
            } else {
                s[k] = 0;
            }
        }
        /* new dp: dp_next[k] = dp[k] + 2 * s[k] */
        for (idx_t k = 0; k <= N; ++k) {
            dp_next[k] = dp[k] + (s[k] * 2);
        }
        /* swap dp and dp_next for next iteration */
        seq_t *tmp = dp; dp = dp_next; dp_next = tmp;
    }

    /* a(n) = Σ_{j≡0 (mod 7), j ≤ n} b(n-j)  =  b(n) + a(n-7) */
    for (idx_t k = 0; k <= N; ++k) {
        if (k < 7) {
            a_val[k] = dp[k];
        } else {
            a_val[k] = dp[k] + a_val[k - 7];
        }
    }

    seq_t result = a_val[N];

    free(dp);
    free(dp_next);
    free(s);
    free(a_val);
    return result;
}
