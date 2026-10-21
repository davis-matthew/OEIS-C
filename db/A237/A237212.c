/*
 * A237212  Number of (n+1)X(1+1) 0..3 arrays with the maximum plus the upper
 *          median minus the lower median minus the minimum of every 2X2
 *          subblock equal.
 * https://oeis.org/A237212
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 4, u32 n <= 11, u64 n <= 25, u128 n <= 51,
 *            big any n
 * Helpers:   compute_sm, vec_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A237212 – number of (n+1)×2 arrays over {0,1,2,3}
    with the property that for every 2×2 subblock the value
    max + upper‑median – lower‑median – min is the same for
    all subblocks.  The algorithm uses an adjacency matrix
    for each possible constant value (0…6) of the expression,
    then counts walks of length n (n steps between n+1 rows)
    in that graph by fast matrix exponentiation. */

#include <stdint.h>
#include "../utils/compute_sm.h"
#include "../utils/vec_mul.h"

static void mat_mul(const seq_t a[16][16], const seq_t b[16][16], seq_t res[16][16])
{
    seq_t temp[16][16];
    for (int i = 0; i < 16; ++i)
        for (int j = 0; j < 16; ++j)
            temp[i][j] = 0;
    for (int i = 0; i < 16; ++i)
        for (int k = 0; k < 16; ++k) {
            if (a[i][k] == 0) continue;
            seq_t aik = a[i][k];
            for (int j = 0; j < 16; ++j)
                temp[i][j] += aik * b[k][j];
        }
    for (int i = 0; i < 16; ++i)
        for (int j = 0; j < 16; ++j)
            res[i][j] = temp[i][j];
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* offset is 1, but return 0 for safety */

    /* ----- precompute adjacency matrices for values 0 … 6 ----- */
    static int initialized = 0;
    static seq_t adj[7][16][16];
    if (!initialized) {
        for (int k = 0; k < 7; ++k) {
            for (int i = 0; i < 16; ++i) {
                int a0 = i >> 2, a1 = i & 3;
                for (int j = 0; j < 16; ++j) {
                    int b0 = j >> 2, b1 = j & 3;
                    adj[k][i][j] = (compute_sm(a0, a1, b0, b1) == k) ? (seq_t)1 : (seq_t)0;
                }
            }
        }
        initialized = 1;
    }

    seq_t total = 0;

    for (int k = 0; k < 7; ++k) {
        /* vector of starting states – all 16 states are allowed */
        seq_t vec[16];
        for (int i = 0; i < 16; ++i) vec[i] = 1;

        /* matrix to be exponentiated – start with adjacency for this k */
        seq_t mat[16][16];
        for (int i = 0; i < 16; ++i)
            for (int j = 0; j < 16; ++j)
                mat[i][j] = adj[k][i][j];

        idx_t exp = n;                  /* number of steps (edges) */
        while (exp) {
            if (exp & 1) {
                seq_t tmp_vec[16];
                vec_mul(vec, mat, tmp_vec);
                for (int j = 0; j < 16; ++j) vec[j] = tmp_vec[j];
            }
            exp >>= 1;
            if (exp) {                 /* square matrix if more bits to process */
                seq_t tmp_mat[16][16];
                mat_mul(mat, mat, tmp_mat);
                for (int i = 0; i < 16; ++i)
                    for (int j = 0; j < 16; ++j)
                        mat[i][j] = tmp_mat[i][j];
            }
        }

        /* sum all entries of the final vector – count for this k */
        seq_t sum = 0;
        for (int i = 0; i < 16; ++i) sum += vec[i];
        total += sum;
    }

    return total;
}
