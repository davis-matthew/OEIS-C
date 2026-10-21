/*
 * A212240  Number of 2 X 2 matrices M with all terms in {1,...,n} and
 *          permanent(M) >= n.
 * https://oeis.org/A212240
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n^4 - Σ_{x=1}^{n-1} R(x) * S(n-1-x) where R(x) = |{(a,d)
 *            ∈ [1,n]^2 : a*d = x}| S(t) = Σ_{i=1}^{t} R(i)
 * Range:     u8 n <= 4, u16 n <= 16, u32 n <= 33, u64 n <= 33, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

static seq_t *R = NULL;
static seq_t *S = NULL;
static size_t cap = 0;

seq_t a(idx_t n)
{
    if (n == 0) return 0;

    /* Ensure internal arrays are large enough */
    if (n > cap) {
        free(R);
        free(S);
        R = (seq_t *)malloc(n * sizeof(seq_t));
        S = (seq_t *)malloc(n * sizeof(seq_t));
        cap = n;
    }

    /* Initialize arrays */
    memset(R, 0, n * sizeof(seq_t));

    /* Compute R(x): number of ordered pairs (a,d) with a*d = x (x <= n-1) */
    idx_t n1 = n - 1;
    for (idx_t a_val = 1; a_val < n; ++a_val) {
        idx_t maxd = n1 / a_val;
        if (maxd > n) maxd = n;
        for (idx_t d = 1; d <= maxd; ++d) {
            idx_t x = a_val * d;          /* guaranteed x <= n-1 */
            ++R[x];
        }
    }

    /* Prefix sums S(t) */
    S[0] = 0;
    seq_t accum = 0;
    for (idx_t i = 1; i < n; ++i) {
        accum += R[i];
        S[i] = accum;
    }

    /* Count matrices with ad + bc < n  (the complement B) */
    seq_t B = 0;
    for (idx_t x = 1; x < n; ++x) {
        B += R[x] * S[n1 - x];
    }

    /* Compute a(n) = n^4 - B */
    seq_t tn = (seq_t)n;
    seq_t n2 = tn * tn;
    seq_t n4 = n2 * n2;
    return n4 - B;
}
