/*
 * A005363  Hoggatt sequence with parameter d=5.
 * https://oeis.org/A005363
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} T(n,k), T(n,k) = Prod_{j=0..4}
 *            binomial(n+j,k)/binomial(k+j,k) (the terms of
 *            Hypergeometric5F4([-4-n,...,-n],[2,3,4,5],-1)); T(n,0) = 1 and
 *            T(n,k) = T(n,k-1) * Prod_{j=0..4} (n+1-k+j)/(k+j)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 10, u64 n <= 18, u128 n <= 33,
 *            big any n
 * Helpers:   g_ull
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/g_ull.h"

seq_t a(idx_t n) {
    seq_t result = 1;
    seq_t term = 1;
    idx_t k;
    for (k = 1; k <= n; k++) {
        unsigned long long num[5], den[5];
        int i, j, iter;
        bool progress;
        for (i = 0; i < 5; i++) {
            num[i] = (unsigned long long)(n + 1 - k + (idx_t)i);
            den[i] = (unsigned long long)(k + (idx_t)i);
        }
        for (iter = 0; iter < 1000; iter++) {
            progress = false;
            for (i = 0; i < 5; i++) {
                for (j = 0; j < 5 && den[i] > 1; j++) {
                    unsigned long long g = g_ull(num[j], den[i]);
                    if (g > 1) { num[j] /= g; den[i] /= g; progress = true; }
                }
                if (den[i] > 1) {
                    seq_t d = (seq_t)den[i];
                    if ((unsigned long long)d == den[i]) {
                        unsigned long long r = (unsigned long long)(term % d);
                        unsigned long long g = g_ull(den[i], r);
                        if (g > 1) {
                            term /= (seq_t)g;
                            den[i] /= g;
                            progress = true;
                        }
                    }
                }
            }
            if (!progress) break;
        }
        for (i = 0; i < 5; i++) term *= (seq_t)num[i];
        result += term;
    }
    return result;
}
