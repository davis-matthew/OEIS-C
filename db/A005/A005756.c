/*
 * A005756  Number of non-degenerate fanout-free Boolean functions of n
 *          variables having AND rank 2.
 * https://oeis.org/A005756
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   Let h(1) = 2, e(0) = 1, e(1) = 2 and, for m >= 2, h(m) =
 *            Sum_{k=1..m-1} binomial(m-1,k-1)*h(k)*e(m-k), e(m) = 2*h(m) (h =
 *            E.g.f. of AND-indecomposable fanout-free functions, H = 2x +
 *            exp(H) - 1 - H, e(m) = m![x^m] exp(H)). Then a(n) = n![x^n]
 *            H^2/2 = Sum_{k=1..n-1} binomial(n-1,k-1)*h(k)*h(n-k)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 14, u128 n <= 24,
 *            big any n
 * Helpers:   alloc_zero, grow_zero
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/alloc_zero.h"
#include "../utils/grow_zero.h"

static seq_t *H = NULL;
static seq_t *E = NULL;
static idx_t N = 0; /* indices 0..N-1 computed */

static void ensure(idx_t n)
{
    idx_t m, k;
    if (n < N) return;
    if (H == NULL) {
        H = alloc_zero(n + 1);
        E = alloc_zero(n + 1);
    } else {
        H = grow_zero(H, N, n + 1);
        E = grow_zero(E, N, n + 1);
    }
    for (m = N; m <= n; m++) {
        if (m == 0) {
            H[0] = 0;
            E[0] = 1;
        } else if (m == 1) {
            H[1] = 2;
            E[1] = 2;
        } else {
            seq_t r = 0;
            seq_t c = 1; /* binomial(m-1, k-1) */
            for (k = 1; k < m; k++) {
                r += c * H[k] * E[m - k];
                c = c * (seq_t)(m - k) / (seq_t)k;
            }
            H[m] = r;
            E[m] = r + r;
        }
    }
    N = n + 1;
}

seq_t a(idx_t n)
{
    idx_t k;
    seq_t s = 0;
    seq_t c = 1; /* binomial(n-1, k-1) */
    if (n < 2) return 0;
    ensure(n - 1);
    for (k = 1; k < n; k++) {
        s += c * H[k] * H[n - k];
        c = c * (seq_t)(n - k) / (seq_t)k;
    }
    return s;
}
