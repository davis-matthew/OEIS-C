/*
 * A025605  Number of n-move queen paths on 8x8 board from given corner to
 *          opposite corner.
 * https://oeis.org/A025605
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 8, u64 n <= 15, u128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) = number of length‑n queen paths on an 8×8 board
 * from one corner to the opposite corner.
 * Computation: use the queen‑graph adjacency matrix (64×64).
 * a(n) = (0,63) entry of Aⁿ where A is the adjacency matrix.
 * We apply binary exponentiation with vector × matrix updates.
 */

#include <stdbool.h>
#include <stddef.h>   /* for size_t */

/* the required function */
seq_t a(idx_t n)
{
    /* a(0) = 0 by definition */
    if (n == 0)
        return 0;

    /* adjacency matrix of the queen graph */
    static seq_t adj[64][64];
    static bool adj_inited = false;

    if (!adj_inited) {
        adj_inited = true;
        /* initialise to 0 (static storage guarantees zero) */
        for (size_t r1 = 0; r1 < 8; ++r1) {
            for (size_t c1 = 0; c1 < 8; ++c1) {
                size_t i = r1 * 8 + c1;
                for (size_t r2 = 0; r2 < 8; ++r2) {
                    for (size_t c2 = 0; c2 < 8; ++c2) {
                        if (r1 == r2 && c1 == c2)
                            continue;
                        int dr = (int)r2 - (int)r1;
                        int dc = (int)c2 - (int)c1;
                        if (dr == 0 || dc == 0 || dr == dc || dr == -dc) {
                            size_t j = r2 * 8 + c2;
                            adj[i][j] = 1;
                        }
                    }
                }
            }
        }
    }

    /* matrices and vectors for exponentiation */
    seq_t power[64][64];
    seq_t temp[64][64];
    seq_t vec[64];
    seq_t newvec[64];

    /* power = adjacency matrix */
    for (size_t i = 0; i < 64; ++i)
        for (size_t j = 0; j < 64; ++j)
            power[i][j] = adj[i][j];

    /* vec = unit vector at start square (index 0) */
    for (size_t i = 0; i < 64; ++i)
        vec[i] = 0;
    vec[0] = 1;

    /* binary exponentiation */
    idx_t exp = n;
    while (exp > 0) {
        if (exp & 1) {
            /* vec = vec * power */
            for (size_t j = 0; j < 64; ++j) {
                seq_t sum = 0;
                for (size_t i = 0; i < 64; ++i)
                    sum += vec[i] * power[i][j];
                newvec[j] = sum;
            }
            for (size_t j = 0; j < 64; ++j)
                vec[j] = newvec[j];
        }

        /* square the matrix: temp = power * power */
        for (size_t i = 0; i < 64; ++i) {
            for (size_t j = 0; j < 64; ++j) {
                seq_t sum = 0;
                for (size_t k = 0; k < 64; ++k)
                    sum += power[i][k] * power[k][j];
                temp[i][j] = sum;
            }
        }
        /* copy temp into power */
        for (size_t i = 0; i < 64; ++i)
            for (size_t j = 0; j < 64; ++j)
                power[i][j] = temp[i][j];

        exp >>= 1;
    }

    /* result: number of paths from start (0) to end (63) */
    return vec[63];
}
