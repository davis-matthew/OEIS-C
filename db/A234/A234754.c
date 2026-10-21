/*
 * A234754  Number of (n+1)X(2+1) 0..3 arrays with 2X2 edge jumps all no more
 *          than +1 in one of the clockwise or counterclockwise directions or
 *          both.
 * https://oeis.org/A234754
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of (n+1)×3 arrays with entries 0..3 satisfying *
 *            that for every 2×2 block the edge jumps along either the
 *            clockwise or * counter‑clockwise direction are all ≤ +1. * This
 *            is computed by dynamic programming over rows via a 64×64
 *            transition * matrix
 * Range:     u8 none, u16 n <= 2, u32 n <= 5, u64 n <= 12, u128 n <= 26,
 *            big any n
 * Helpers:   encode_row, mat_mul, valid_block, vec_mat_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Required types: seq_t (wide integer), idx_t (uint64_t) */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/encode_row.h"
#include "../utils/mat_mul.h"
#include "../utils/valid_block.h"
#include "../utils/vec_mat_mul.h"

/* precomputed transition matrix for allowed row transitions */
static seq_t T[64][64];
static bool T_init = false;

/* initialise T once */
static void init_matrix(void) {
    if (T_init) return;
    for (idx_t i = 0; i < 64; ++i) {
        int a = (i >> 4) & 3;
        int b = (i >> 2) & 3;
        int c = i & 3;
        for (idx_t j = 0; j < 64; ++j) {
            int x = (j >> 4) & 3;
            int y = (j >> 2) & 3;
            int z = j & 3;
            /* both 2×2 blocks must be valid */
            if (valid_block(a, b, x, y) && valid_block(b, c, y, z))
                T[i][j] = 1;
            else
                T[i][j] = 0;
        }
    }
    T_init = true;
}

/* main sequence function */
seq_t a(idx_t n) {
    init_matrix();                 /* ensure T is ready           */
    if (n == 0) return 0;          /* not used, but safe          */

    /* vector of counts for current state */
    seq_t vec[64];
    for (idx_t i = 0; i < 64; ++i)
        vec[i] = 1;                 /* any first row is allowed    */

    /* matrix powered on the fly using binary exponentiation */
    seq_t power[64][64], tmp[64][64];
    for (idx_t i = 0; i < 64; ++i)
        for (idx_t j = 0; j < 64; ++j)
            power[i][j] = T[i][j];

    idx_t bits = n;
    while (bits > 0) {
        if (bits & 1ULL) {
            seq_t vec2[64];
            vec_mat_mul(vec, power, vec2);
            /* copy back */
            for (idx_t i = 0; i < 64; ++i)
                vec[i] = vec2[i];
        }
        /* square the matrix */
        mat_mul(power, power, tmp);
        for (idx_t i = 0; i < 64; ++i)
            for (idx_t j = 0; j < 64; ++j)
                power[i][j] = tmp[i][j];

        bits >>= 1ULL;
    }

    /* sum over all final rows */
    seq_t result = 0;
    for (idx_t i = 0; i < 64; ++i)
        result += vec[i];

    return result;
}
