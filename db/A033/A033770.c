/*
 * A033770  Product t2(q^d); d | 11, where t2 = theta2(q)/(2*q^(1/4)).
 * https://oeis.org/A033770
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of positive odd solutions to x^2 + 11*y^2 = 8*n +
 *            12
 * Range:     u8 n <= 7, u16 n <= 1636, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/floor_sqrt.h"

/* Computes the n-th term of OEIS sequence A033770. */
seq_t a(idx_t n)
{
    /* S = 8*n + 12 */
    seq_t S = ((seq_t)8) * n + ((seq_t)12);
    /* maximum possible odd y: floor(sqrt(S/11)) */
    seq_t y_max = floor_sqrt(S / ((seq_t)11));

    seq_t count = 0;
    for (seq_t y = 1; y <= y_max; y += 2) {
        /* R = S - 11*y^2 */
        seq_t y2   = y * y;
        seq_t sub  = ((seq_t)11) * y2;
        seq_t R    = S - sub;
        if (R <= (seq_t)0)
            continue;
        seq_t x = floor_sqrt(R);
        if (x * x != R)
            continue;
        /* x is automatically odd because R ≡ 1 (mod 8) */
        if (x & ((seq_t)1))
            ++count;
    }
    return count;
}
