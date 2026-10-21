/*
 * A237060  Number of (n+1)X(1+1) 0..2 arrays with the maximum plus the upper
 *          median plus the minimum minus the lower median of every 2X2
 *          subblock equal.
 * https://oeis.org/A237060
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of (n+1)×2 arrays over {0,1,2} such that the
 *            quantity * max + upper-median + min – lower-median * of every
 *            2×2 subblock is the same for all subblocks. * This is computed
 *            by counting walks of length n in a 9-state graph * (the states
 *            are the two-entry rows) where edges correspond to...
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 14, u64 n <= 31, u128 n <= 65,
 *            big any n
 * Helpers:   f_of_block
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>   /* for size_t */
#include "../utils/f_of_block.h"

/* 9×9 matrix of seq_t values */
typedef seq_t mat_t[9][9];

/* matrix multiplication: res = a * b */
static void mat_mul(const mat_t a, const mat_t b, mat_t res) {
    for (idx_t i = 0; i < 9; ++i) {
        for (idx_t j = 0; j < 9; ++j) {
            seq_t s = 0;
            for (idx_t k = 0; k < 9; ++k) {
                s += a[i][k] * b[k][j];
            }
            res[i][j] = s;
        }
    }
}

/* fast exponentiation of a matrix */
static void mat_pow(const mat_t base, idx_t exp, mat_t result) {
    /* initialize result as identity */
    for (idx_t i = 0; i < 9; ++i)
        for (idx_t j = 0; j < 9; ++j)
            result[i][j] = (i == j) ? 1 : 0;

    mat_t power;
    /* copy base to power */
    for (idx_t i = 0; i < 9; ++i)
        for (idx_t j = 0; j < 9; ++j)
            power[i][j] = base[i][j];

    mat_t temp;
    while (exp) {
        if (exp & 1) {
            mat_mul(result, power, temp);
            for (idx_t i = 0; i < 9; ++i)
                for (idx_t j = 0; j < 9; ++j)
                    result[i][j] = temp[i][j];
        }
        exp >>= 1;
        if (!exp) break;  /* skip last square */
        mat_mul(power, power, temp);
        for (idx_t i = 0; i < 9; ++i)
            for (idx_t j = 0; j < 9; ++j)
                power[i][j] = temp[i][j];
    }
}

/* main sequence function */
seq_t a(idx_t n) {
    static const int state[9][2] = {
        {0,0},{0,1},{0,2},{1,0},{1,1},{1,2},{2,0},{2,1},{2,2}
    };

    /* precompute f for all transitions */
    int fmat[9][9];
    for (idx_t i = 0; i < 9; ++i) {
        for (idx_t j = 0; j < 9; ++j) {
            fmat[i][j] = f_of_block(state[i][0], state[i][1],
                                    state[j][0], state[j][1]);
        }
    }

    seq_t total = 0;

    for (int C = 0; C <= 4; ++C) {
        mat_t adj;
        /* build adjacency matrix for this C */
        for (idx_t i = 0; i < 9; ++i) {
            for (idx_t j = 0; j < 9; ++j) {
                adj[i][j] = (fmat[i][j] == C) ? 1 : 0;
            }
        }

        mat_t powM;
        mat_pow(adj, n, powM);

        /* sum all entries of powM */
        seq_t sumC = 0;
        for (idx_t i = 0; i < 9; ++i)
            for (idx_t j = 0; j < 9; ++j)
                sumC += powM[i][j];

        total += sumC;
    }

    return total;
}
