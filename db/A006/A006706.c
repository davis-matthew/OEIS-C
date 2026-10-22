/*
 * A006706  Self-convolution of numbers of trees on n nodes.
 * https://oeis.org/A006706
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{i=0..n} A000055(i)*A000055(n-i), where A000055(0) =
 *            1 and for n >= 1 A000055(n) = A000081(n) - Sum_{1<=i<n/2}
 *            A000081(i)*A000081(n-i) - [n even]*binomial(A000081(n/2)+1-1,
 *            2), with A000081 (rooted trees) from the Euler product: 1 +
 *            Sum_{n>=1} A000081(n+1) x^n = Prod_{k>=1} 1/(1-x^k)^A...
 * Range:     u8 n <= 9, u16 n <= 15, u32 n <= 27, u64 n <= 49, u128 n <= 91,
 *            big any n
 * Helpers:   alloc_zero, release
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/alloc_zero.h"
#include "../utils/release.h"

static seq_t *T = NULL;   /* unrooted tree counts t[0..TL-1] */
static idx_t TL = 0;

static void build(idx_t L) {
    seq_t *F = alloc_zero(L);
    seq_t *r = alloc_zero(L);
    seq_t *c = alloc_zero(L);
    idx_t k, i, j, n;
    F[0] = 1;
    for (k = 1; k < L; k++) {
        r[k] = F[k - 1];
        if (L >= 2 && k <= L - 2) {
            idx_t D = L - 2;
            idx_t jmax = D / k;
            seq_t m = r[k];
            c[0] = 1;
            for (j = 1; j <= jmax; j++)
                c[j] = c[j - 1] * (m + (seq_t)(j - 1)) / (seq_t)j;
            for (i = D; i >= k; i--) {
                seq_t s = 0;
                for (j = 0; j * k <= i; j++)
                    s += c[j] * F[i - j * k];
                F[i] = s;
            }
        }
    }
    T = alloc_zero(L);
    T[0] = 1;
    for (n = 1; n < L; n++) {
        seq_t s = 0;
        for (i = 1; 2 * i < n; i++)
            s += r[i] * r[n - i];
        if (n % 2 == 0) {
            seq_t x = r[n / 2];
            if (x % 2 == 0) s += (x / 2) * (x - 1);
            else s += x * ((x - 1) / 2);
        }
        T[n] = r[n] - s;
    }
    release(F, L);
    release(r, L);
    release(c, L);
}

seq_t a(idx_t n) {
    if (n >= TL) {
        idx_t newL = TL + TL / 2;
        if (newL < n + 1) newL = n + 1;
        if (newL < 8) newL = 8;
        if (T != NULL) release(T, TL);
        T = NULL;
        build(newL);
        TL = newL;
    }
    seq_t result = 0;
    for (idx_t i = 0; i <= n; i++)
        result += T[i] * T[n - i];
    return result;
}
