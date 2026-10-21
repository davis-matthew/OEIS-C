/*
 * A227494  The Wiener index of the nanostar dendrimer D_n, defined
 *          pictorially as NS[n] in the Yang et al. reference.
 * https://oeis.org/A227494
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4983 + 2^n*(15048*n + 29502) + 4^n*(14256*n - 33048)
 * Range:     u8 none, u16 n <= 1, u32 n <= 7, u64 n <= 17, u128 n <= 17,
 *            big any n
 * Helpers:   pow2, pow4
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"
#include "../utils/pow4.h"

seq_t a(idx_t n)
{
    seq_t sn   = (seq_t)n;

    /* 2^n */
    seq_t pow2n = pow2(n);
    /* 4^n */
    seq_t pow4n = pow4(n);

    /* 2^n * (15048*n + 29502) */
    seq_t part1 = ((seq_t)15048) * sn + (seq_t)29502;
    seq_t term1 = pow2n * part1;

    /* 4^n * (14256*n - 33048) */
    seq_t part2 = ((seq_t)14256) * sn - (seq_t)33048;
    seq_t term2 = pow4n * part2;

    return (seq_t)4983 + term1 + term2;
}
