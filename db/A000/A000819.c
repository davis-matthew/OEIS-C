/*
 * A000819  E.g.f.: cos(x)^2 / cos(2x) = Sum_{n >= 0} a(n) * x^(2n) / (2n)!.
 * https://oeis.org/A000819
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2^(2n-1) * A000364(n) for n >= 1, a(0) = 1, where
 *            A000364(n) = A000111(2n) is computed with the Seidel-Entringer
 *            boustrophedon triangle: E(0,0) = 1, E(k,0) = 0 for k > 0, E(k,m)
 *            = E(k,m-1) + E(k-1,k-m), A000111(k) = E(k,k)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 5, u64 n <= 9, u128 n <= 15,
 *            big any n
 * Helpers:   alloc_zero, release
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/alloc_zero.h"
#include "../utils/release.h"

seq_t a(idx_t n) {
    idx_t K, k, m, i;
    seq_t *prev, *cur, *tmp;
    seq_t e, p, result;

    if (n == 0) return 1;

    K = 2 * n;
    prev = alloc_zero(K + 1);
    cur = alloc_zero(K + 1);

    prev[0] = 1;
    for (k = 1; k <= K; k++) {
        cur[0] = 0;
        for (m = 1; m <= k; m++)
            cur[m] = cur[m - 1] + prev[k - m];
        tmp = prev;
        prev = cur;
        cur = tmp;
    }
    e = prev[K];

    release(prev, K + 1);
    release(cur, K + 1);

    p = 1;
    for (i = 0; i + 1 < K; i++)
        p *= 2;

    result = e * p;
    return result;
}
