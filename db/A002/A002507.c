/*
 * A002507  Expansion of a modular function for Gamma_0(6).
 * https://oeis.org/A002507
 *
 * Offset:    -3    (a(-3) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = [q^(n+3)] eta(q^2)^10 * eta(q^3)^14 / (eta(q)^2 *
 *            eta(q^6)^22), where eta(q) = prod_{k>=1} (1 - q^k) here
 *            (q^(1/24) factor dropped). eta is applied via the pentagonal
 *            series eta(q) = sum (-1)^k q^(k(3k-1)/2) and the Jacobi series
 *            eta(q)^3 = sum_{k>=0} (-1)^k (2k+1) q^(k(k+1)/2)
 * Range:     i8 n <= 2, i16 n <= 11, i32 n <= 47, i64 n <= 173, i128 n <= 659,
 *            big any n
 * Helpers:   a2507_apply, a2507_tail
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>
#include "../utils/a2507_apply.h"
#include "../utils/a2507_tail.h"

seq_t a(idx_t n)
{
    static seq_t *cache = 0;
    static idx_t cap = 0;
    idx_t m = n + 3;
    if (m < 0) return 0;
    if (m >= cap) {
        idx_t L = 2 * cap;
        idx_t i;
        seq_t *f;
        if (L < m + 1) L = m + 1;
        if (L < 64) L = 64;
        f = (seq_t *)malloc((size_t)L * sizeof(seq_t));
        for (i = 0; i < L; i++) f[i] = 0;
        f[0] = 1;
        /* eta(q^2)^10 = (eta(q^2)^3)^3 * eta(q^2) */
        a2507_apply(f, L, 2, 1, 0, 3);
        a2507_apply(f, L, 2, 0, 0, 1);
        /* eta(q^3)^14 = (eta(q^3)^3)^4 * eta(q^3)^2 */
        a2507_apply(f, L, 3, 1, 0, 4);
        a2507_apply(f, L, 3, 0, 0, 2);
        /* / eta(q)^2 */
        a2507_apply(f, L, 1, 0, 1, 2);
        /* / eta(q^6)^22 = / ((eta(q^6)^3)^7 * eta(q^6)) */
        a2507_apply(f, L, 6, 1, 1, 7);
        a2507_apply(f, L, 6, 0, 1, 1);
        if (cache) free(cache);
        cache = f;
        cap = L;
    }
    return cache[m];
}
