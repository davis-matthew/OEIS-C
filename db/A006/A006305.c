/*
 * A006305  Taylor series related to one in Ramanujan's Lost Notebook.
 * https://oeis.org/A006305
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   G.f.: Sum_{k>=0} q^(k^2+k) * P_k(q), where P_0 = 1/(1-q)^2 and
 *            P_k = P_{k-1} * (1+q^(2k)) / ((1-q^(2k)) * (1-q^(2k+1))^2). Each
 *            P_k is built from P_{k-1} in O(n), truncated to degree n-(k^2+k)
 * Range:     u8 n <= 12, u16 n <= 33, u32 n <= 102, u64 n <= 343,
 *            u128 n <= 1227, big any n
 * Helpers:   alloc_zero, release
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include "../utils/alloc_zero.h"
#include "../utils/release.h"

static seq_t *A006305_cache = NULL;
static idx_t A006305_len = 0;

static void A006305_compute(idx_t N) {
    seq_t *P = alloc_zero(N + 1);
    seq_t *res = alloc_zero(N + 1);
    idx_t i, k;
    P[0] = 1;
    /* divide by (1-q)^2 */
    for (int t = 0; t < 2; t++)
        for (i = 1; i <= N; i++) P[i] += P[i - 1];

    for (k = 0; k * (k + 1) <= N; k++) {
        idx_t s = k * (k + 1);
        idx_t L = N - s;
        if (k > 0) {
            idx_t m = 2 * k;
            /* multiply by (1+q^m) */
            if (L >= m)
                for (i = L; i >= m; i--) P[i] += P[i - m];
            /* divide by (1-q^m) */
            for (i = m; i <= L; i++) P[i] += P[i - m];
            /* divide by (1-q^(m+1))^2 */
            for (int t = 0; t < 2; t++)
                for (i = m + 1; i <= L; i++) P[i] += P[i - (m + 1)];
        }
        for (i = 0; i <= L; i++) res[s + i] += P[i];
    }
    release(P, N + 1);
    if (A006305_cache != NULL) release(A006305_cache, A006305_len);
    A006305_cache = res;
    A006305_len = N + 1;
}

seq_t a(idx_t n) {
    if (n >= A006305_len) {
        idx_t N = n;
        if (N < 2 * A006305_len) N = 2 * A006305_len;
        A006305_compute(N);
    }
    return A006305_cache[n];
}
