/*
 * A000958  Number of ordered rooted trees with n edges having root of odd
 *          degree.
 * https://oeis.org/A000958
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = A000957(n) + A000957(n+1), where G(k) = A000957(k+1)
 *            satisfies G(0) = 1, G(k) = (A000108(k) - G(k-1))/2; for
 *            fixed-width seq_t, uses the equivalent g.f. z*C/(1-z^2*C^2):
 *            a(n) = Sum_{j>=0} [z^(n-1-2j)] C^(2j+1), with [z^m]C^k computed
 *            by additions only
 * Range:     u8 n <= 7, u16 n <= 11, u32 n <= 20, u64 n <= 36, u128 n <= 69,
 *            big any n
 * Helpers:   a958_big, a958_is_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/a958_big.h"
#include "../utils/a958_is_big.h"

#define A958_MAXN 200
static seq_t a958_prev[A958_MAXN + 3];
static seq_t a958_cur[A958_MAXN + 3];

static seq_t a958_fixed(idx_t n) {
    /* B(m,k) = [z^m] C^k ; B(0,k)=1, B(m,0)=0 (m>=1), B(m,k)=B(m,k-1)+B(m-1,k+1) */
    idx_t m, k;
    seq_t sum = 0;
    if (n > A958_MAXN) return 0; /* far beyond any fixed-width range */
    for (k = 0; k <= n + 1; k++) a958_prev[k] = 1;
    /* m = 0 row: contributes if n-1 = 2j, k = 2j+1 = n */
    if ((n - 1) % 2 == 0) sum += a958_prev[n];
    for (m = 1; m <= n - 1; m++) {
        a958_cur[0] = 0;
        for (k = 1; k + m <= n; k++)
            a958_cur[k] = a958_cur[k - 1] + a958_prev[k + 1];
        /* need k = n - m with k odd, i.e. n-1-m even */
        if ((n - 1 - m) % 2 == 0) sum += a958_cur[n - m];
        for (k = 0; k + m <= n; k++) a958_prev[k] = a958_cur[k];
    }
    return sum;
}

seq_t a(idx_t n) {
    if (n == 0) return 0;
    if (a958_is_big()) return a958_big(n);
    return a958_fixed(n);
}
