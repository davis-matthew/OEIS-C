/*
 * A045634  Number of ways in which n can be partitioned as a sum of a square
 *          and cube.
 * https://oeis.org/A045634
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 63, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Counting the number of ways n can be written as a sum of a square and a cube.
   a(n) = #{(x, y) ∈ ℕ₀² : x² + y³ = n} */
#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    /* count solutions */
    seq_t count = (seq_t)0;
    for (idx_t y = 0; ; ++y) {
        /* y³ in seq_t arithmetic */
        seq_t yseq = (seq_t)y;
        seq_t cube = yseq * yseq * yseq;
        if (cube > n)            /* cube exceeds n, no more solutions possible */
            break;
        seq_t rem = (seq_t)n - cube;
        seq_t root = floor_sqrt(rem);
        if (root * root == rem)  /* if the remainder is a perfect square */
            ++count;
    }
    return count;
}
