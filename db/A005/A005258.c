/*
 * A005258  Apéry numbers: a(n) = Sum_{k=0..n} binomial(n,k)^2 *
 *          binomial(n+k,k).
 * https://oeis.org/A005258
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice, changed
 * Formula:   a(n) = Sum_{k=0..n} binomial(n,k)^2 * binomial(n+k,k)
 *            (fixed-width builds, via Pascal's triangle); (n+1)^2*a(n+1) =
 *            (11*n^2+11*n+3)*a(n) + n^2*a(n-1), a(0) = 1, a(1) = 3 (arbitrary
 *            precision builds)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 10, u64 n <= 20, u128 n <= 38,
 *            big any n
 * Helpers:   seq_is_fixed_width
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/seq_is_fixed_width.h"

#define A005258_PMAX 200

static seq_t a005258_row[2 * A005258_PMAX + 2];
static seq_t a005258_bn[A005258_PMAX + 1];

/* Exact modulo 2^w: uses only additions and multiplications. */
static seq_t a005258_pascal(idx_t n) {
    idx_t m, j, k;
    seq_t sum = 0;
    for (j = 0; j <= 2 * n + 1; j++) a005258_row[j] = 0;
    a005258_row[0] = 1;
    for (m = 0; m <= 2 * n; m++) {
        if (m > 0) {
            for (j = m; j >= 1; j--)
                a005258_row[j] = a005258_row[j] + a005258_row[j - 1];
        }
        if (m == n) {
            for (k = 0; k <= n; k++) a005258_bn[k] = a005258_row[k];
        }
        if (m >= n) {
            k = m - n;
            sum += a005258_bn[k] * a005258_bn[k] * a005258_row[k];
        }
    }
    return sum;
}

seq_t a(idx_t n) {
    idx_t m;
    seq_t prev, cur;
    if (seq_is_fixed_width() && n <= A005258_PMAX)
        return a005258_pascal(n);
    if (n == 0) return 1;
    prev = 1;
    cur = 3;
    for (m = 1; m < n; m++) {
        seq_t M = (seq_t)m;
        seq_t M1 = M + 1;
        seq_t next = ((M * M * 11 + M * 11 + 3) * cur + M * M * prev) / (M1 * M1);
        prev = cur;
        cur = next;
    }
    return cur;
}
