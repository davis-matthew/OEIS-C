/*
 * A003711  Expansion of e.g.f. cos(tanh(x)) (even powers only).
 * https://oeis.org/A003711
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = (2n)! [x^(2n)] cos(tanh(x)). Using integer EGF
 *            coefficients (c_n = sum_k binomial(n,k) a_k b_{n-k} for
 *            products): T = tanh(x): T(0) = 0, T(m+1) = [m=0] - Sum_{k=0..m}
 *            binomial(m,k) T(k) T(m-k); s = 1 - tanh(x)^2 = tanh'(x): s(m) =
 *            T(m+1); F = cos(tanh x), G = sin(tanh x): F(0) = 1, G(0) =...
 * Range:     i8 n <= 2, i16 n <= 4, i32 n <= 6, i64 n <= 11, i128 n <= 18,
 *            big any n
 * Helpers:   alloc_zero, grow_zero
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/alloc_zero.h"
#include "../utils/grow_zero.h"

static seq_t *T = 0, *S = 0, *F = 0, *G = 0, *B = 0;
static idx_t cap = 0;
static idx_t done = 0; /* F,G,T valid for indices 0..done; B holds binomial row 'done' */

static void ensure(idx_t N) {
    if (cap < N + 2) {
        idx_t newcap = cap ? cap : 16;
        while (newcap < N + 2) newcap *= 2;
        if (cap == 0) {
            T = alloc_zero(newcap);
            S = alloc_zero(newcap);
            F = alloc_zero(newcap);
            G = alloc_zero(newcap);
            B = alloc_zero(newcap);
            F[0] = 1;
            B[0] = 1;
        } else {
            T = grow_zero(T, cap, newcap);
            S = grow_zero(S, cap, newcap);
            F = grow_zero(F, cap, newcap);
            G = grow_zero(G, cap, newcap);
            B = grow_zero(B, cap, newcap);
        }
        cap = newcap;
    }
    while (done < N) {
        idx_t m = done;
        idx_t k;

        /* T(m+1): nonzero only when m+1 is odd, i.e. m even */
        if (m % 2 == 0) {
            seq_t acc = 0;
            for (k = 1; k < m; k += 2)
                acc += B[k] * T[k] * T[m - k];
            seq_t t = 0;
            if (m == 0) t = 1;
            T[m + 1] = t - acc;
        } else {
            T[m + 1] = 0;
        }
        S[m] = T[m + 1];

        if (m % 2 == 1) {
            /* F(m+1): G(k) nonzero for odd k, s(m-k) nonzero for even m-k */
            seq_t acc = 0;
            for (k = 1; k <= m; k += 2)
                acc += B[k] * G[k] * S[m - k];
            F[m + 1] = -acc;
            G[m + 1] = 0;
        } else {
            /* G(m+1): F(k) nonzero for even k */
            seq_t acc = 0;
            for (k = 0; k <= m; k += 2)
                acc += B[k] * F[k] * S[m - k];
            G[m + 1] = acc;
            F[m + 1] = 0;
        }

        /* advance binomial row m -> m+1 */
        for (k = m + 1; k >= 1; k--)
            B[k] += B[k - 1];

        done++;
    }
}

seq_t a(idx_t n) {
    idx_t N = 2 * n;
    ensure(N);
    return F[N];
}
