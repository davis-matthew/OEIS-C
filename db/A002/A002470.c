/*
 * A002470  Glaisher's function W(n).
 * https://oeis.org/A002470
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = [q^n] q*theta_3(q)^2*eta(q^2)^12, i.e.
 *            rho^7*(eta(q)*eta(q^4)/eta(q^2)^2)^24 with eta^3 = Sum (-1)^k
 *            (2k+1) q^(k(k+1)/2)
 * Range:     i8 n <= 5, i16 n <= 31, i32 n <= 850, i64 n <= 850,
 *            i128 n <= 850, big any n
 * Helpers:   from_i128, w_zeros
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/from_i128.h"
#include "../utils/w_zeros.h"

static __int128 *W_cache = NULL;
static idx_t W_len = 0;

static void w_compute(idx_t L) {
    idx_t M = (L - 1) / 2 + 1;
    __int128 *T = w_zeros(M);
    idx_t *pos = (idx_t *)malloc((size_t)(M + 1) * sizeof(idx_t));
    idx_t np = 0;
    for (idx_t k = 0;; k++) {
        idx_t t = k * (k + 1) / 2;
        if (t >= M) break;
        __int128 v = (__int128)(2 * k + 1);
        if (k & 1) v = -v;
        T[t] = v;
        pos[np++] = t;
    }
    /* eta^6 = (eta^3)^2 */
    __int128 *S = w_zeros(M);
    for (idx_t i = 0; i < np; i++)
        for (idx_t j = 0; j < np; j++) {
            idx_t s = pos[i] + pos[j];
            if (s < M) S[s] += T[pos[i]] * T[pos[j]];
        }
    /* eta^12 = (eta^6)^2 */
    __int128 *E = w_zeros(M);
    for (idx_t m = 0; m < M; m++) {
        __int128 s = 0;
        for (idx_t i = 0; 2 * i < m; i++) s += S[i] * S[m - i];
        s *= 2;
        if ((m & 1) == 0) s += S[m / 2] * S[m / 2];
        E[m] = s;
    }
    __int128 *b = w_zeros(L);
    for (idx_t j = 0; 2 * j < L; j++) b[2 * j] = E[j];
    /* multiply twice by theta_3(q) = 1 + 2*Sum q^{s^2} */
    for (int rep = 0; rep < 2; rep++) {
        for (idx_t k = L; k-- > 0;) {
            __int128 acc = 0;
            for (idx_t s = 1; s * s <= k; s++) acc += b[k - s * s];
            b[k] += 2 * acc;
        }
    }
    free(T); free(pos); free(S); free(E);
    if (W_cache) free(W_cache);
    W_cache = b;
    W_len = L;
}

seq_t a(idx_t n) {
    if (n == 0) return 0;
    if (n > W_len) {
        idx_t L = 2 * W_len;
        if (L < n + 64) L = n + 64;
        w_compute(L);
    }
    return from_i128(W_cache[n - 1]);
}
