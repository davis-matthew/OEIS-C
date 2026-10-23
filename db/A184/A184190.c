/*
 * A184190  Half the number of (n+1) X 3 binary arrays with no 2 X 2 subblock
 *          containing exactly one 1.
 * https://oeis.org/A184190
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ½ * sum_{i,j} (M^n)_{i,j} , where M is the 8×8 adjacency
 *            matrix of 3‑bit rows: M[r][s] = 1 iff no 2×2 subblock formed by
 *            rows r and s contains exactly one ‘1’. This yields the sequence
 *            A184190
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 25, u128 n <= 51,
 *            big any n
 * Helpers:   init_adjacent
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/init_adjacent.h"

static void mat_mul(const seq_t A[8][8], const seq_t B[8][8], seq_t C[8][8]) {
    for (uint32_t i = 0; i < 8; ++i)
        for (uint32_t j = 0; j < 8; ++j) {
            seq_t sum = 0;
            for (uint32_t k = 0; k < 8; ++k)
                sum += A[i][k] * B[k][j];
            C[i][j] = sum;
        }
}

seq_t a(idx_t n) {
    if (n < 1)
        return 0;              /* undefined for n < 1, return 0 */

    /* Build adjacency matrix once. */
    static seq_t M[8][8];
    init_adjacent(M);

    /* Fast exponentiation of M to the power n. */
    seq_t res[8][8];
    seq_t base[8][8];
    seq_t tmp[8][8];
    for (uint32_t i = 0; i < 8; ++i)
        for (uint32_t j = 0; j < 8; ++j)
            res[i][j] = (i == j) ? 1 : 0;
    for (uint32_t i = 0; i < 8; ++i)
        for (uint32_t j = 0; j < 8; ++j)
            base[i][j] = M[i][j];

    idx_t exp = n;
    while (exp) {
        if (exp & 1ULL) {
            mat_mul(res, base, tmp);
            for (uint32_t i = 0; i < 8; ++i)
                for (uint32_t j = 0; j < 8; ++j)
                    res[i][j] = tmp[i][j];
        }
        mat_mul(base, base, tmp);
        for (uint32_t i = 0; i < 8; ++i)
            for (uint32_t j = 0; j < 8; ++j)
                base[i][j] = tmp[i][j];
        exp >>= 1;
    }

    /* Sum all entries of M^n and divide by two. */
    seq_t total = 0;
    for (uint32_t i = 0; i < 8; ++i)
        for (uint32_t j = 0; j < 8; ++j)
            total += res[i][j];
    return total / 2;
}
