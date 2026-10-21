/*
 * A228687  Number of 6Xn binary arrays with no two ones adjacent
 *          horizontally, diagonally or antidiagonally.
 * https://oeis.org/A228687
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of 6×n binary arrays with no two neighbouring 1s
 *            horizontally, diagonally or antidiagonally
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 18, u128 n <= 37,
 *            big any n
 * Helpers:   compat
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/compat.h"

seq_t a(idx_t n) {
    if (n == 0) return 0;          /* offset starts at 1, guard against n==0 */

    /* initial vector of length‑1 column states: each of the 64 patterns occurs once */
    seq_t vec[64];
    for (int i = 0; i < 64; ++i) vec[i] = 1;

    if (n == 1) {
        seq_t total = 0;
        for (int i = 0; i < 64; ++i) total += vec[i];
        return total;              /* 64 */
    }

    /* Build adjacency matrix (size 64×64) once */
    static seq_t M0[64][64];
    static bool initialized = false;
    if (!initialized) {
        for (int a = 0; a < 64; ++a) {
            for (int b = 0; b < 64; ++b) {
                M0[a][b] = compat((uint8_t)a, (uint8_t)b) ? 1 : 0;
            }
        }
        initialized = true;
    }

    /* Create mutable copy of M0 for exponentiation */
    seq_t M[64][64];
    for (int i = 0; i < 64; ++i)
        for (int j = 0; j < 64; ++j)
            M[i][j] = M0[i][j];

    /* Exponentiation by squaring: vec ← vec · M^(n‑1) */
    idx_t exp = n - 1;
    seq_t tmp_vec[64];
    seq_t tmp_mat[64][64];

    while (exp > 0) {
        if (exp & 1) {
            for (int j = 0; j < 64; ++j) {
                seq_t sum = 0;
                for (int i = 0; i < 64; ++i)
                    sum += vec[i] * M[i][j];
                tmp_vec[j] = sum;
            }
            for (int j = 0; j < 64; ++j)
                vec[j] = tmp_vec[j];
        }

        /* M ← M · M */
        for (int i = 0; i < 64; ++i) {
            for (int j = 0; j < 64; ++j) {
                seq_t sum = 0;
                for (int k = 0; k < 64; ++k)
                    sum += M[i][k] * M[k][j];
                tmp_mat[i][j] = sum;
            }
        }
        for (int i = 0; i < 64; ++i)
            for (int j = 0; j < 64; ++j)
                M[i][j] = tmp_mat[i][j];

        exp >>= 1;
    }

    /* Sum all entries of the final vector: this is a(n) */
    seq_t total = 0;
    for (int i = 0; i < 64; ++i)
        total += vec[i];
    return total;
}
