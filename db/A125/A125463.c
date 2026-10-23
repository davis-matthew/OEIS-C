/*
 * A125463  Number of base 28 circular n-digit numbers with adjacent digits
 *          differing by 8 or less.
 * https://oeis.org/A125463
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(0) = 1 * For n ≥ 1: a(n) = trace(Mⁿ), where M is the 28×28
 *            matrix * with M[i][j] = 1 if |i – j| ≤ 8, else 0. * * This
 *            implements the OEIS sequence A125463. * All arithmetic uses the
 *            seq_t type supplied by the build; * the implementation is fully
 *            compliant with C11/C++17
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 8, u64 n <= 16, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

#define B 28                    /* number of possible digits (0‑27) */

/* --- matrix helpers ---------------------------------------------- */
static void mat_identity(seq_t R[B][B]) {
    for (int i = 0; i < B; ++i)
        for (int j = 0; j < B; ++j)
            R[i][j] = (i == j) ? 1 : 0;
}

/* multiply A * B, store in R */
static void mat_mul(const seq_t A[B][B], const seq_t Bmat[B][B], seq_t R[B][B]) {
    for (int i = 0; i < B; ++i) {
        for (int j = 0; j < B; ++j) {
            seq_t s = 0;
            for (int k = 0; k < B; ++k)
                s += A[i][k] * Bmat[k][j];
            R[i][j] = s;
        }
    }
}

/* --- main sequence function ------------------------------------- */
seq_t a(idx_t n) {
    /* static adjacency matrix */
    static bool init_done = false;
    static seq_t adj[B][B];

    if (!init_done) {
        for (int i = 0; i < B; ++i) {
            for (int j = 0; j < B; ++j) {
                int diff = (i > j) ? i - j : j - i;
                adj[i][j] = (diff <= 8) ? 1 : 0;
            }
        }
        init_done = true;
    }

    if (n == 0)                 /* by convention */
        return 1;

    seq_t result[B][B];          /* current result matrix (identity initially) */
    mat_identity(result);

    seq_t power[B][B];
    for (int i = 0; i < B; ++i)
        for (int j = 0; j < B; ++j)
            power[i][j] = adj[i][j];

    idx_t exp = n;
    while (exp) {
        if (exp & 1) {           /* multiply result by current power */
            seq_t tmp[B][B];
            mat_mul(result, power, tmp);
            for (int i = 0; i < B; ++i)
                for (int j = 0; j < B; ++j)
                    result[i][j] = tmp[i][j];
        }
        exp >>= 1;
        if (exp) {               /* square the power matrix */
            seq_t tmp[B][B];
            mat_mul(power, power, tmp);
            for (int i = 0; i < B; ++i)
                for (int j = 0; j < B; ++j)
                    power[i][j] = tmp[i][j];
        }
    }

    /* compute trace of M^n */
    seq_t trace = 0;
    for (int i = 0; i < B; ++i)
        trace += result[i][i];

    return trace;
}
