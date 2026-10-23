/*
 * A110261  Number of distinct numbers that can be written as
 *          floor(n/i)+floor(n/j), 1<=i<=j<=n.
 * https://oeis.org/A110261
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of distinct values of floor(n/i)+floor(n/j) for 1
 *            ≤ i ≤ j ≤ n
 * Range:     u8 n <= 72, u16 n <= 72, u32 n <= 72, u64 n <= 72, u128 n <= 72,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* Skip the undefined offset region (n < 1) – it is irrelevant. */
    if (n < 1) return 0;

    /* ---- Collect distinct values of floor(n/k) for k = 1..n ---- */
    idx_t *vals = NULL;
    size_t cap = 0, sz = 0;
    for (idx_t k = 1; k <= n; ) {
        idx_t v   = n / k;          /* current quotient */
        idx_t last = n / v;         /* largest k that gives this v */

        if (sz == cap) {
            cap = cap ? cap << 1 : 16;
            idx_t *tmp = (idx_t *)realloc(vals, cap * sizeof(idx_t));
            if (!tmp) { free(vals); return 0; }
            vals = tmp;
        }
        vals[sz++] = v;
        k = last + 1;
    }

    /* ---- Mark all achievable sums vi + vj (i ≤ j) ---- */
    size_t range = (size_t)n * 2 + 1;            /* sums lie in [0, 2n] */
    uint8_t *present = (uint8_t *)calloc(range, 1);
    if (!present) { free(vals); return 0; }

    seq_t count = 0;
    for (size_t i = 0; i < sz; ++i) {
        idx_t vi = vals[i];
        for (size_t j = i; j < sz; ++j) {
            size_t sum = (size_t)vi + (size_t)vals[j];
            if (!present[sum]) {
                present[sum] = 1;
                ++count;
            }
        }
    }

    free(vals);
    free(present);

    return count;
}
