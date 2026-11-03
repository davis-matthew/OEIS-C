/*
 * A177894  Determinant of the square matrix whose rows are the cyclic
 *          permutations of the digits of n.
 * https://oeis.org/A177894
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, base, easy, look
 * Formula:   a(n) = determinant of the square matrix whose rows are the
 *            cyclic permutations of the digits of n
 * Range:     i8 n <= 105, i16 n <= 9999, i32 n <= 9999, i64 n <= 9999,
 *            i128 n <= 9999, big any n
 * Validated: standard, pass, 114 terms through n = 8925 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static seq_t det_matrix(const seq_t *mat, int n)
{
    if (n == 1)
        return mat[0];

    seq_t result = 0;
    seq_t minor[25];        /* buffer for minors (enough for at most 5×5). */

    for (int j = 0; j < n; ++j) {
        /* Build the minor excluding row 0 and column j. */
        int mi = 0;
        for (int i = 1; i < n; ++i) {
            int mj = 0;
            for (int k = 0; k < n; ++k) {
                if (k == j)
                    continue;
                minor[mi * (n - 1) + mj] = mat[i * n + k];
                ++mj;
            }
            ++mi;
        }

        seq_t subdet = det_matrix(minor, n - 1);
        seq_t term = mat[0 * n + j] * subdet;
        if (j & 1) term = -term;   /* alternating sign. */
        result += term;
    }

    return result;
}

static seq_t det_matrix(const seq_t *mat, int n);

seq_t a(idx_t n)
{
    /* n == 0 is a single‑digit 0 matrix. */
    if (n == 0)
        return (seq_t)0;

    /* Extract the decimal digits of n into an array, most significant first. */
    int digits[5];          /* 5 digits are enough for n ≤ 99999 (≈10000). */
    int len = 0;
    idx_t tmp = n;
    while (tmp > 0) {
        digits[len++] = (int)(tmp % 10);
        tmp /= 10;
    }
    /* Re‑order to most significant first. */
    for (int i = 0; i < len / 2; ++i) {
        int t = digits[i];
        digits[i] = digits[len - 1 - i];
        digits[len - 1 - i] = t;
    }

    /* Build the circulant matrix of size len × len. */
    seq_t M[25];             /* 5 × 5 maximum. */
    for (int i = 0; i < len; ++i) {
        for (int j = 0; j < len; ++j) {
            M[i * len + j] = (seq_t)digits[(i + j) % len];
        }
    }

    return det_matrix(M, len);
}
