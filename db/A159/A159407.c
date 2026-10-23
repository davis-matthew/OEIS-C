/*
 * A159407  Number of n X n arrays of squares of integers summing to 21.
 * https://oeis.org/A159407
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = sum over all solutions (c1,c4,c9,c16) of (n*n choose c1)
 *            * (n*n-c1 choose c4) * (n*n-c1-c4 choose c9) * (n*n-c1-c4-c9
 *            choose c16) where c1*1 + c4*4 + c9*9 + c16*16 = 21. The
 *            remaining cells contain 0²
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 5, u64 n <= 8, u128 n <= 23,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>
#include "../utils/binom.h"

seq_t a(idx_t n)
{
    if (n < 2) return (seq_t)0;

    idx_t cells = n * n;

    struct { idx_t c1, c4, c9, c16; } const solutions[] = {
        {5,0,0,1},
        {1,1,0,1},
        {21,0,0,0},
        {17,1,0,0},
        {13,2,0,0},
        {9,3,0,0},
        {5,4,0,0},
        {1,5,0,0},
        {12,0,1,0},
        {8,1,1,0},
        {4,2,1,0},
        {0,3,1,0},
        {3,0,2,0}
    };
    size_t num = sizeof(solutions) / sizeof(solutions[0]);

    seq_t total = 0;
    for (size_t i = 0; i < num; ++i) {
        const idx_t used = solutions[i].c1 + solutions[i].c4
                         + solutions[i].c9 + solutions[i].c16;
        if (used > cells) continue;

        idx_t rem = cells;
        seq_t prod = 1;

        prod = prod * binom(rem, solutions[i].c1);
        rem -= solutions[i].c1;

        prod = prod * binom(rem, solutions[i].c4);
        rem -= solutions[i].c4;

        prod = prod * binom(rem, solutions[i].c9);
        rem -= solutions[i].c9;

        prod = prod * binom(rem, solutions[i].c16);

        total += prod;
    }
    return total;
}
