/*
 * A100112  If n is the k-th squarefree number then a(n) = k, otherwise a(n) =
 *          0.
 * https://oeis.org/A100112
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = if n is the k-th squarefree number then a(n)=k, otherwise
 *            0
 * Range:     u8 n <= 418, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 100 terms through n = 9982 (2026-11-01)
 * Helpers:   isqrt, mobius_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/isqrt.h"
#include "../utils/mobius_idx.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;  /* index 0 is not used, safety */

    /* Check if n is squarefree. */
    if (mobius_idx(n) == 0) return 0;

    /* Count squarefree numbers <= n via Möbius inversion. */
    idx_t sqrt_n = isqrt(n);
    int64_t count = 0;
    for (idx_t k = 1; k <= sqrt_n; ++k) {
        int mu = mobius_idx(k);
        if (mu == 0) continue;
        idx_t kk = k * k;
        idx_t term = n / kk;
        count += (int64_t)mu * (int64_t)term;
    }
    return (seq_t)count;
}
