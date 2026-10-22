/*
 * A000039  Coefficient of q^(2n) in the series expansion of Ramanujan's mock
 *          theta function f(q).
 * https://oeis.org/A000039
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = [q^(2n)] f(q), where f(q) = Sum_{k>=0} q^(k^2) /
 *            Product_{j=1..k} (1+q^j)^2, evaluated in Horner form: S_K = 1,
 *            S_{k-1} = 1 + q^(2k-1) * S_k / (1+q^k)^2, f = S_0
 * Range:     i8 n <= 16, i16 n <= 54, i32 n <= 192, i64 n <= 699,
 *            i128 n <= 2629, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *a39_c = NULL;
static long long a39_len = 0;

static void a39_build(long long M)
{
    long long L = M + 1, i, k, K = 0, s;
    seq_t *S = (seq_t *)malloc((size_t)L * sizeof(seq_t));
    while ((K + 1) * (K + 1) <= M) K++;
    for (i = 0; i < L; i++) S[i] = 0;
    S[0] = 1;
    for (k = K; k >= 1; k--) {
        /* divide by (1+q^k) twice */
        for (i = k; i < L; i++) S[i] -= S[i - k];
        for (i = k; i < L; i++) S[i] -= S[i - k];
        /* multiply by q^(2k-1) */
        s = 2 * k - 1;
        for (i = L - 1; i >= s; i--) S[i] = S[i - s];
        for (i = (s < L ? s : L) - 1; i >= 0; i--) S[i] = 0;
        /* add 1 */
        S[0] += 1;
    }
    if (a39_c) free(a39_c);
    a39_c = S;
    a39_len = L;
}

seq_t a(idx_t n)
{
    long long M = 2 * (long long)n;
    if (a39_c == NULL || M >= a39_len) {
        long long target = M;
        if (a39_len > 0 && 2 * a39_len > target) target = 2 * a39_len;
        if (target < 64) target = 64;
        a39_build(target);
    }
    return a39_c[M];
}
