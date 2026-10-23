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
 * Helpers:   det_matrix
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/det_matrix.h"

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
