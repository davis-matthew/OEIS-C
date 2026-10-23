/*
 * A125330  Number of base 20 circular n-digit numbers with adjacent digits
 *          differing by 3 or less.
 * https://oeis.org/A125330
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = 1, if n == 0; otherwise a(n) = trace(A^n) where A is the
 *            20×20 adjacency matrix with A[i][j] = 1 if |i - j| ≤ 3, else 0.
 *            (0‑based digits)
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 19, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>     /* may be needed for certain builds */

#define B 20   /* number of digits in base 20 */

static void mat_mul(const seq_t A[B][B], const seq_t Bmat[B][B], seq_t C[B][B])
{
    for (int i = 0; i < B; ++i) {
        for (int j = 0; j < B; ++j) {
            seq_t sum = 0;
            for (int k = 0; k < B; ++k) {
                sum += A[i][k] * Bmat[k][j];
            }
            C[i][j] = sum;
        }
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 1;

    /* adjacency matrix (one-time construction) */
    static seq_t base[B][B];
    static int init = 0;
    if (!init) {
        for (int i = 0; i < B; ++i) {
            for (int j = 0; j < B; ++j) {
                int diff = i - j;
                if (diff < 0) diff = -diff;
                base[i][j] = (diff <= 3) ? 1 : 0;
            }
        }
        init = 1;
    }

    /* Fast exponentiation of matrix base^n */
    seq_t result[B][B];
    seq_t cur[B][B];
    seq_t temp[B][B];

    /* identity matrix */
    for (int i = 0; i < B; ++i) {
        for (int j = 0; j < B; ++j) {
            result[i][j] = (i == j) ? 1 : 0;
            cur[i][j] = base[i][j];
        }
    }

    idx_t exp = n;
    while (exp > 0) {
        if (exp & 1) {
            mat_mul(result, cur, temp);
            for (int i = 0; i < B; ++i)
                for (int j = 0; j < B; ++j)
                    result[i][j] = temp[i][j];
        }
        mat_mul(cur, cur, temp);
        for (int i = 0; i < B; ++i)
            for (int j = 0; j < B; ++j)
                cur[i][j] = temp[i][j];

        exp >>= 1;
    }

    /* trace of result matrix */
    seq_t trace = 0;
    for (int i = 0; i < B; ++i)
        trace += result[i][i];

    return trace;
}
