/*
 * A003719  Expansion of tan(x)*cosh(x).
 * https://oeis.org/A003719
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} binomial(2n+1, 2k)*A000182(n+1-k), where
 *            A000182(j) are the tangent numbers (A000111(2j-1)), computed by
 *            the Brent-Harvey recurrence T(1) = 1, T(k) = (k-1)*T(k-1), then
 *            for k>=2, j>=k: T(j) = (j-k)*T(j-1) + (j-k+2)*T(j)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 6, u64 n <= 11, u128 n <= 15,
 *            big any n
 * Helpers:   alloc_zero, release, seq_is_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/alloc_zero.h"
#include "../utils/release.h"
#include "../utils/seq_is_big.h"

static seq_t *T = NULL;   /* T[1..Tn] tangent numbers */
static idx_t Tn = 0;

static void build_T(idx_t N) {
    idx_t newN, k, j;
    seq_t *t;
    if (N <= Tn) return;
    newN = (Tn * 2 > N) ? Tn * 2 : N;
    t = alloc_zero(newN + 1);
    t[1] = 1;
    for (k = 2; k <= newN; k++)
        t[k] = t[k - 1] * (seq_t)(k - 1);
    for (k = 2; k <= newN; k++)
        for (j = k; j <= newN; j++)
            t[j] = t[j - 1] * (seq_t)(j - k) + t[j] * (seq_t)(j - k + 2);
    if (T) release(T, Tn + 1);
    T = t;
    Tn = newN;
}

seq_t a(idx_t n) {
    idx_t m = 2 * n + 1;
    idx_t k, i, j;
    seq_t result = 0;

    build_T(n + 1);

    if (seq_is_big()) {
        seq_t b = 1; /* binomial(m, j) */
        for (j = 0; j <= m; j++) {
            if ((j & 1) == 0)
                result += b * T[n + 1 - j / 2];
            if (j < m)
                b = b * (seq_t)(m - j) / (seq_t)(j + 1);
        }
    } else {
        seq_t *row = alloc_zero(m + 1);
        row[0] = 1;
        for (i = 1; i <= m; i++)
            for (j = i; j >= 1; j--)
                row[j] += row[j - 1];
        for (k = 0; k <= n; k++)
            result += row[2 * k] * T[n + 1 - k];
        release(row, m + 1);
    }
    return result;
}
