/*
 * A227488  The Wiener index of the nanostar dendrimer NSC_5C_6[n], defined
 *          pictorially in the Ashrafi et al. and the Rostami et al.
 *          references.
 * https://oeis.org/A227488
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = -13612 + 2^n*(72*n + 45270) + 4^n*(12960*n - 31680)
 * Range:     u8 none, u16 n <= 1, u32 n <= 7, u64 n <= 22, u128 n <= 54,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    /* Handle n < 1 silently (offset is 1). */
    if (n < 1)
        return 0;

    seq_t two_pow  = big_pow((seq_t)2, (seq_t)n);
    seq_t four_pow = big_pow((seq_t)4, (seq_t)n);

    /* Positive contributions. */
    seq_t pos = two_pow  * (((seq_t)72 * n) + 45270);
    pos += four_pow * (((seq_t)12960 * n));

    /* Negative contributions. */
    seq_t neg = four_pow * 31680 + (seq_t)13612;

    /* Since pos >= neg for all n >= 1, unsigned subtraction is safe. */
    return pos - neg;
}
