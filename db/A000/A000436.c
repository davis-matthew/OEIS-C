/*
 * A000436  Generalized Euler numbers c(3,n).
 * https://oeis.org/A000436
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, changed
 * Formula:   G.f. as Jacobi continued fraction 1/(1 - 2*4*x/(1 - 6*6*x/(1 -
 *            8*10*x/(1 - 12*12*x/(1 - ...))))), i.e. a(n) = weighted
 *            Motzkin-free (Dyck) path sum of length 2n with down-step weights
 *            lam(2j+1) = (6j+2)(6j+4), lam(2j) = (6j)^2; a(0) = 1
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 5, u64 n <= 8, u128 n <= 14,
 *            big any n
 * Helpers:   lam
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/lam.h"

#define A436_MAXN 12000

static seq_t a436_T[A436_MAXN + 3];
static seq_t a436_cache[A436_MAXN + 1];
static idx_t a436_cache_len = 0;

seq_t a(idx_t n) {
    if (n < a436_cache_len)
        return a436_cache[n];
    if (n > A436_MAXN)
        return 0;

    idx_t N = n;
    seq_t *T = a436_T;
    for (idx_t i = 0; i < N + 3; i++)
        T[i] = 0;
    T[0] = 1;
    a436_cache[0] = 1;

    idx_t total = 2 * N;
    for (idx_t s = 0; s < total; s++) {
        idx_t t = s + 1;                      /* new step index */
        idx_t hi = t;
        if (total - t < hi) hi = total - t;   /* heights that can still return */
        idx_t h = t % 2;
        for (; h <= hi; h += 2) {
            seq_t v = T[h + 1] * lam(h + 1);
            if (h >= 1)
                v += T[h - 1];
            T[h] = v;
        }
        if (t % 2 == 0)
            a436_cache[t / 2] = T[0];
    }

    a436_cache_len = N + 1;
    return a436_cache[n];
}
