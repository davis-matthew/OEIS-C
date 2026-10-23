/*
 * A125361  Number of base 24 circular n-digit numbers with adjacent digits
 *          differing by 4 or less.
 * https://oeis.org/A125361
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = trace(A^n) for n≥1 where A[i][j]=1 if |i−j|≤4 (indices
 *            0…23); a(0) = 1. The trace is the sum of the diagonal of the
 *            n‑th power of the adjacency matrix *A*
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 10, u64 n <= 19, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>

/* matrix multiplication (24×24) – only allowed operators are used */
static void mat_mult(const seq_t a[24][24], const seq_t b[24][24], seq_t res[24][24])
{
    for (int i = 0; i < 24; ++i) {
        for (int j = 0; j < 24; ++j) {
            seq_t sum = 0;
            for (int k = 0; k < 24; ++k) {
                sum += a[i][k] * b[k][j];
            }
            res[i][j] = sum;
        }
    }
}

seq_t a(idx_t n)
{
    if (n == 0) {
        return (seq_t)1;
    }

    /* initialise adjacency matrix once */
    static seq_t adj[24][24];
    static bool init = false;
    if (!init) {
        for (int i = 0; i < 24; ++i) {
            for (int j = 0; j < 24; ++j) {
                int diff = i - j;
                if (diff < 0) diff = -diff;
                adj[i][j] = (diff <= 4) ? (seq_t)1 : (seq_t)0;
            }
        }
        init = true;
    }

    /* exponentiation by squaring */
    seq_t res[24][24];  /* will hold A^n */
    seq_t base[24][24]; /* current power of A */
    for (int i = 0; i < 24; ++i) {
        for (int j = 0; j < 24; ++j) {
            res[i][j] = (i == j) ? (seq_t)1 : (seq_t)0; /* identity */
            base[i][j] = adj[i][j];
        }
    }

    idx_t exp = n;
    seq_t tmp[24][24];
    while (exp > 0) {
        if ((exp & (idx_t)1) != 0) {
            mat_mult(res, base, tmp);
            for (int i = 0; i < 24; ++i)
                for (int j = 0; j < 24; ++j)
                    res[i][j] = tmp[i][j];
        }
        exp >>= 1;
        if (exp == 0) break;
        mat_mult(base, base, tmp);
        for (int i = 0; i < 24; ++i)
            for (int j = 0; j < 24; ++j)
                base[i][j] = tmp[i][j];
    }

    /* compute trace of A^n */
    seq_t trace = 0;
    for (int i = 0; i < 24; ++i) {
        trace += res[i][i];
    }
    return trace;
}
