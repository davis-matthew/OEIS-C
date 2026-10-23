/*
 * A125385  Number of base 22 circular n-digit numbers with adjacent digits
 *          differing by 5 or less.
 * https://oeis.org/A125385
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 17, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = 1 for n = 0
 *               trace(A^n)  for n >= 1
 * Adjacency matrix A of size 22x22:
 *   A[i][j] = 1 if |i-j| <= 5, otherwise 0.
 * The trace of A^n counts the number of base‑22 circular n‑digit numbers
 * whose adjacent digits differ by at most 5.
 */

#include <stddef.h>   /* for size_t */

#define BASE 22

/* local helper: matrix multiplication */
static void mat_mul(const seq_t (*a)[BASE], const seq_t (*b)[BASE], seq_t (*res)[BASE])
{
    for (int i = 0; i < BASE; ++i) {
        for (int j = 0; j < BASE; ++j) {
            seq_t sum = 0;
            for (int k = 0; k < BASE; ++k) {
                seq_t prod = a[i][k] * b[k][j];
                sum += prod;
            }
            res[i][j] = sum;
        }
    }
}

/* local helper: identity matrix */
static void mat_identity(seq_t (*id)[BASE])
{
    for (int i = 0; i < BASE; ++i) {
        for (int j = 0; j < BASE; ++j) {
            id[i][j] = (i == j) ? (seq_t)1 : (seq_t)0;
        }
    }
}

/* the required sequence function */
seq_t a(idx_t n)
{
    if (n == 0) {
        return (seq_t)1;
    }

    /* adjacency matrix A */
    static seq_t A[BASE][BASE];
    static int initialized = 0;
    if (!initialized) {
        for (int i = 0; i < BASE; ++i) {
            for (int j = 0; j < BASE; ++j) {
                /* |i-j| <= 5 */
                int diff = i - j;
                if (diff < 0) diff = -diff;
                A[i][j] = (diff <= 5) ? (seq_t)1 : (seq_t)0;
            }
        }
        initialized = 1;
    }

    /* matrix exponentiation: result = A^n */
    seq_t result[BASE][BASE];
    seq_t base[BASE][BASE];
    seq_t temp[BASE][BASE];

    mat_identity(result);
    /* copy A into base */
    for (int i = 0; i < BASE; ++i)
        for (int j = 0; j < BASE; ++j)
            base[i][j] = A[i][j];

    idx_t exp = n;
    while (exp > 0) {
        if (exp & 1) {
            mat_mul(result, base, temp);
            /* copy temp back to result */
            for (int i = 0; i < BASE; ++i)
                for (int j = 0; j < BASE; ++j)
                    result[i][j] = temp[i][j];
        }
        mat_mul(base, base, temp);
        for (int i = 0; i < BASE; ++i)
            for (int j = 0; j < BASE; ++j)
                base[i][j] = temp[i][j];
        exp >>= 1;
    }

    /* trace of result */
    seq_t trace = 0;
    for (int i = 0; i < BASE; ++i) {
        trace += result[i][i];
    }
    return trace;
}
