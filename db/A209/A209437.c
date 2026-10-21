/*
 * A209437  Table of T(m,n), read by antidiagonals, is the number of subsets
 *          of {1,...,n} which contain two elements whose difference is m.
 * https://oeis.org/A209437
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   T(m,n) = 2^n - ∏_{i=0}^{m-1} F(floor((n+i)/m + 2)) a(n) is read
 *            by antidiagonals of the table T(m,n) (m≥1, n≥2). The mapping is
 *            s = smallest integer with (s-2)(s-1)/2 ≥ n pos = n -
 *            (s-3)(s-2)/2 - 1 (0‑based in the diagonal) m = (s-2) - pos n₂ =
 *            pos + 2 Then a(n) = T(m,n₂). This implementation use...
 * Range:     u8 n <= 35, u16 n <= 135, u32 n <= 527, u64 n <= 2079,
 *            u128 n <= 5050, big any n
 * Helpers:   fib, pow2, seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/fib.h"
#include "../utils/pow2.h"
#include "../utils/seq_mul.h"

seq_t a(idx_t n) {
    /* find diagonal sum s */
    idx_t s = 3;
    for (;;) {
        __int128 c = (__int128)(s - 2) * (s - 1) / 2;
        if (c >= n) break;
        ++s;
    }
    /* position inside the diagonal */
    idx_t before = 0;
    if (s > 3) {
        __int128 b = (__int128)(s - 3) * (s - 2) / 2;
        before = (idx_t)b;
    }
    idx_t pos = n - before - 1;
    /* compute corresponding m, n₂ */
    idx_t m = (s - 2) - pos;
    idx_t n2 = pos + 2;

    /* compute T(m,n2) */
    seq_t pow2_n = pow2(n2);
    seq_t prod   = 1;
    for (idx_t i = 0; i < m; ++i) {
        idx_t val_n = (n2 + i) / m;  /* floor division */
        val_n += 2;
        seq_t f = fib(val_n);
        prod = seq_mul(prod, f);
    }
    return pow2_n - prod;
}
