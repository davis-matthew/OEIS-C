/*
 * A000685  Number of 3-colored labeled graphs on n nodes, divided by 3.
 * https://oeis.org/A000685
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = (1/3)*Sum_{j=0..n} binomial(n,j)*2^(j*(n-j))*c(j), where
 *            c(m) = Sum_{i=0..m} binomial(m,i)*2^(i*(m-i)) = A047863(m)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 12, u128 n <= 17,
 *            big any n
 * Helpers:   a685_fixed, a685_pow2, alloc_zero
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/alloc_zero.h"
#include "../utils/a685_fixed.h"
#include "../utils/a685_pow2.h"

seq_t a(idx_t n) {
    int fixed = a685_fixed();
    seq_t *row = alloc_zero(n + 2);
    seq_t *c = alloc_zero(n + 1);
    seq_t s = 0;
    idx_t i, j;

    row[0] = 1;
    for (j = 0; j <= n; j++) {
        /* row holds binomial(j, i) */
        seq_t t = 0;
        for (i = 0; i <= j; i++)
            t += row[i] * a685_pow2(i * (j - i), fixed);
        c[j] = t;
        for (i = j + 1; i >= 1; i--)
            row[i] += row[i - 1];
    }
    /* row now holds binomial(n+1, i); rebuild binomial(n, j) */
    for (i = 0; i <= n + 1; i++) row[i] = 0;
    row[0] = 1;
    for (j = 0; j < n; j++)
        for (i = j + 1; i >= 1; i--)
            row[i] += row[i - 1];

    for (j = 0; j <= n; j++)
        s += row[j] * a685_pow2(j * (n - j), fixed) * c[j];

    free(row);
    free(c);

    if (fixed) {
        /* exact division by 3 modulo 2^w via inverse of 3 */
        seq_t inv = 3;
        int k;
        for (k = 0; k < 8; k++)
            inv = (seq_t)(inv * (seq_t)((seq_t)2 - (seq_t)((seq_t)3 * inv)));
        return (seq_t)(s * inv);
    }
    return s / 3;
}
