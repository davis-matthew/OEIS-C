/*
 * A012509  Expansion of e.g.f. -log(cos(x)*cos(x)) (even powers only).
 * https://oeis.org/A012509
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2 * A000182(n) (tangent numbers)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 12, u128 n <= 17,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(0) = 0, a(1) = 2, a(2) = 4, ...   Even–power expansion of –log(cos^2(x)). */

#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
#include "../utils/binom.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;

    static idx_t max_computed = 0;
    static seq_t *tan = NULL;
    static size_t cap = 0;

    /* Expand cache if necessary. */
    if (n > max_computed) {
        size_t new_cap = (size_t)n + 1;
        if (new_cap > cap) {
            seq_t *tmp = (seq_t *)realloc(tan, new_cap * sizeof(seq_t));
            if (!tmp) return 0;  /* allocation failure */
            tan = tmp;
            cap = new_cap;
        }

        /* initialise first term if this is the first call. */
        if (max_computed == 0) {
            tan[0] = 0;          /* unused, keep for 1‑based indexing */
            tan[1] = 1;          /* T₁ = 1 */
            max_computed = 1;
        }

        /* compute tangent numbers up to n using recurrence
           T_{m+1} = Σ_{k=1}^{m} binom(2m,2k−1) * T_k * T_{m+1−k} */
        for (idx_t m = max_computed; m < n; ++m) {
            seq_t sum = 0;
            for (idx_t k = 1; k <= m; ++k) {
                seq_t coef = binom((idx_t)(2 * m), (idx_t)(2 * k - 1));
                seq_t temp = coef * tan[k];
                temp = temp * tan[m + 1 - k];
                sum += temp;
            }
            tan[m + 1] = sum;
        }
        max_computed = n;
    }

    /* a(n) = 2 * T_n */
    return tan[n] * 2;
}
