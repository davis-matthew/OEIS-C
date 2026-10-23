/*
 * A125473  Number of base 16 circular n-digit numbers with adjacent digits
 *          differing by 9 or less.
 * https://oeis.org/A125473
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 16, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A125473: Number of base 16 circular n‑digit numbers
 * with adjacent digits differing by 9 or less.
 * a(0) = 1;
 * For n ≥ 1, a(n) = Tr(A^n),
 * where A[i][j] = 1 if |i‑j| ≤ 9, else 0 (0 ≤ i,j < 16). */
#include <stdbool.h>

typedef struct { seq_t m[16][16]; } Mat;

static inline void mat_identity(Mat *dest)
{
    for (int i = 0; i < 16; ++i) {
        for (int j = 0; j < 16; ++j) {
            dest->m[i][j] = (i == j) ? (seq_t)1 : (seq_t)0;
        }
    }
}

static inline void mat_copy(Mat *dest, const Mat *src)
{
    for (int i = 0; i < 16; ++i)
        for (int j = 0; j < 16; ++j)
            dest->m[i][j] = src->m[i][j];
}

static inline void mat_mul(const Mat *a, const Mat *b, Mat *res)
{
    for (int i = 0; i < 16; ++i) {
        for (int j = 0; j < 16; ++j) {
            seq_t tmp = 0;
            for (int k = 0; k < 16; ++k)
                tmp += a->m[i][k] * b->m[k][j];
            res->m[i][j] = tmp;
        }
    }
}

static inline seq_t mat_trace(const Mat *mat)
{
    seq_t sum = 0;
    for (int i = 0; i < 16; ++i)
        sum += mat->m[i][i];
    return sum;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;

    /* Build adjacency matrix once */
    static bool built = false;
    static Mat adj;
    if (!built) {
        for (int i = 0; i < 16; ++i) {
            for (int j = 0; j < 16; ++j) {
                int diff = (i >= j) ? (i - j) : (j - i);
                adj.m[i][j] = (diff <= 9) ? (seq_t)1 : (seq_t)0;
            }
        }
        built = true;
    }

    Mat result, base, tmp;
    mat_identity(&result);
    mat_copy(&base, &adj);

    idx_t exp = n;
    while (exp > 0) {
        if (exp & 1) {
            mat_mul(&result, &base, &tmp);
            mat_copy(&result, &tmp);
        }
        mat_mul(&base, &base, &tmp);
        mat_copy(&base, &tmp);
        exp >>= 1;
    }

    return mat_trace(&result);
}
