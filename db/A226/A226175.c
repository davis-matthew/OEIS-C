/*
 * A226175  a(n) = A068336(n+1) - 1.
 * https://oeis.org/A226175
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k|n} A068336(k), with A068336(1) = 1, A068336(m+1) =
 *            1 + Sum_{k|m} A068336(k) (equivalently a(n) = A068336(n+1) - 1)
 * Range:     u8 n <= 19, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Validated: partial, pass, 50 terms through n = 50 (2026-11-07)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/mem.h"

seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;
    seq_t *s = mem_new(seq_t, (size_t)n + 1);
    seq_t b = (seq_t)1;               /* A068336(m) */
    for (idx_t m = 1; m <= n; ++m) {
        for (idx_t j = m; j <= n; j += m)
            s[j] += b;
        b = s[m] + 1;                 /* A068336(m+1) */
    }
    seq_t r = s[n];
    mem_free(s);
    return r;
}
