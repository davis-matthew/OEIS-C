/*
 * A056311  Number of reversible strings with n beads using exactly four
 *          different colors.
 * https://oeis.org/A056311
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 12 * (S2(n,4) + S2(ceil(n/2),4)) where S2(n,4) = (4^n -
 *            4*3^n + 6*2^n - 4) / 24
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 16, u64 n <= 32, u128 n <= 64,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Requires the helper power function:
   seq_t big_pow(seq_t base, seq_t exp);  (see utils/big_pow.h)  */
#include <stdbool.h>
#include "../utils/big_pow.h"

/* Stirling numbers of the second kind for k = 4 */
static seq_t stirling_n4(idx_t n)
{
    /* Compute 4^n , 3^n , 2^n  */
    seq_t p4 = big_pow((seq_t)4, (seq_t)n);
    seq_t p3 = big_pow((seq_t)3, (seq_t)n);
    seq_t p2 = big_pow((seq_t)2, (seq_t)n);

    /* numerator: 4^n - 4*3^n + 6*2^n - 4 */
    seq_t num = p4 - (seq_t)4 * p3 + (seq_t)6 * p2 - (seq_t)4;

    /* division by 24 (exact) */
    return num / (seq_t)24;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 0)               /* not used, defensive */
        return (seq_t)0;

    seq_t s_n   = stirling_n4(n);
    idx_t half  = (n + 1) / 2;              /* ceil(n/2) */
    seq_t s_half = stirling_n4(half);

    return (seq_t)12 * (s_n + s_half);
}
