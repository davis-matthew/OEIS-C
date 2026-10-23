/*
 * A144735  Square of triangle A054533 (matrix square), read by rows.
 * https://oeis.org/A144735
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  tabl, sign, more
 * Formula:   a(n) is the nth element of the lower‑triangular matrix square of
 *            A054533. If r,c are the 1‑based row and column of the element,
 *            a(n) = Σ_{s=c..r} R(r,s) * R(s,c), where R(n,s) = Σ_{d |
 *            gcd(n,s)} d · μ(n/d) (μ = Möbius function)
 * Range:     i8 n <= 55, i16 n <= 55, i32 n <= 55, i64 n <= 55, i128 n <= 55,
 *            big any n
 * Helpers:   _floor_isqrt_u64, mobius, seq_alloc_array, seq_free_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/mobius.h"
#include "../utils/_floor_isqrt_u64.h"
#include "../utils/seq_alloc_array.h"
#include "../utils/seq_free_array.h"

/* Cache of the R table: R[n][s] for 1 ≤ s ≤ n. */
static seq_t **R = NULL;
static idx_t R_max = 0;

/* Euclidean gcd for idx_t values. */
static inline idx_t idx_gcd(idx_t a, idx_t b)
{
    while (b) {
        idx_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

/* Ensure that R contains rows up to n (inclusive). */
static void ensure_R_rows(idx_t n)
{
    if (n <= R_max)
        return;

    /* Allocate a new table large enough for all needed rows. */
    idx_t newmax = n;
    seq_t **newR = (seq_t**)malloc((newmax + 1) * sizeof(seq_t*));
    if (!newR) exit(EXIT_FAILURE);

    /* Allocate rows 1..newmax. */
    for (idx_t i = 1; i <= newmax; ++i)
        newR[i] = seq_alloc_array(i + 1);

    /* Copy existing rows into the new table. */
    for (idx_t i = 1; i <= R_max; ++i)
        for (idx_t j = 1; j <= i; ++j)
            newR[i][j] = R[i][j];

    /* Compute the new rows that are missing. */
    for (idx_t m = R_max + 1; m <= newmax; ++m) {
        for (idx_t s = 1; s <= m; ++s) {
            idx_t g = idx_gcd(m, s);
            seq_t sum = 0;

            /* Enumerate all divisors d of g. */
            for (idx_t d = 1; d * d <= g; ++d) {
                if (g % d == 0) {
                    idx_t d2 = g / d;
                    sum += (seq_t)d * (seq_t)mobius(m / d);
                    if (d2 != d)
                        sum += (seq_t)d2 * (seq_t)mobius(m / d2);
                }
            }
            newR[m][s] = sum;
        }
    }

    /* Free the old table. */
    for (idx_t i = 1; i <= R_max; ++i)
        seq_free_array(R[i]);
    free(R);

    /* Replace old table with the new one. */
    R = newR;
    R_max = newmax;
}

/* Public function required by the problem statement. */
seq_t a(idx_t n)
{
    if (n < 1)
        return 0;

    /* Find row r such that r*(r+1)/2 ≥ n. */
    idx_t t = _floor_isqrt_u64(8 * n + 1);
    if (t * t < 8 * n + 1)
        ++t;                      /* ceil sqrt */
    idx_t r = (t - 1) / 2;
    if (r * (r + 1) / 2 < n) ++r;

    /* Column position c (1‑based) within that row. */
    idx_t start = r * (r - 1) / 2;
    idx_t c = n - start;

    /* Ensure the table R contains needed rows. */
    ensure_R_rows(r);

    /* Compute T(r,c) = Σ_{s=c..r} R(r,s) * R(s,c). */
    seq_t val = 0;
    for (idx_t s = c; s <= r; ++s)
        val += R[r][s] * R[s][c];

    return val;
}
