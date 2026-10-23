/*
 * A104468  Coefficients of the B-Bailey Mod 9 identity.
 * https://oeis.org/A104468
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = coefficient of the generating function ∏_{k≥1}
 *            (1−x^(9k−2))(1−x^(9k−7)) / ((1−x^(9k−3))(1−x^(9k−6)))
 * Range:     i8 n <= 41, i16 n <= 122, i32 n <= 383, i64 n <= 1304,
 *            i128 n <= 4700, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#ifdef __cplusplus
#include <vector>
#else
#include <stdlib.h>
#endif

seq_t a(idx_t n)
{
    /* Handle trivial case */
    if (n == (idx_t)0)
        return (seq_t)1;

    /* Allocate working arrays */
#ifdef __cplusplus
    std::vector<seq_t> cur(n + 1, 0), temp(n + 1, 0);
#else
    seq_t *cur = (seq_t *)malloc((n + 1) * sizeof(seq_t));
    seq_t *temp = (seq_t *)malloc((n + 1) * sizeof(seq_t));
    if (!cur || !temp) return (seq_t)0;   /* drop out on allocation failure */
    for (idx_t i = 0; i <= n; ++i) cur[i] = temp[i] = 0;
#endif

    cur[0] = (seq_t)1;          /* initial series = 1 */

    idx_t kmax = (n + 7) / 9;   /* largest k with 9k-7 ≤ n */

    for (idx_t k = 1; k <= kmax; ++k) {
        idx_t r1 = 9 * k - 2;
        idx_t r2 = 9 * k - 7;
        idx_t m1 = 9 * k - 3;
        idx_t m2 = 9 * k - 6;

#ifdef __cplusplus
        /* copy cur into temp */
        std::copy(cur.begin(), cur.end(), temp.begin());
#else
        for (idx_t i = 0; i <= n; ++i) temp[i] = cur[i];
#endif

        /* Division by (1 - x^m1) */
        if (m1 <= n) {
            for (idx_t i = m1; i <= n; ++i)
                temp[i] += temp[i - m1];
        }
        /* Division by (1 - x^m2) */
        if (m2 <= n) {
            for (idx_t i = m2; i <= n; ++i)
                temp[i] += temp[i - m2];
        }

        /* Multiply by (1 - x^r1)(1 - x^r2) */
        for (idx_t i = 0; i <= n; ++i) {
            seq_t val = temp[i];
            if (i >= r1) val -= temp[i - r1];
            if (i >= r2) val -= temp[i - r2];
            if (i >= r1 + r2) val += temp[i - r1 - r2];
            cur[i] = val;
        }
    }

    seq_t result = cur[n];

#ifndef __cplusplus
    free(cur);
    free(temp);
#endif

    return result;
}
