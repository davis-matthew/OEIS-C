/*
 * A221003  The hyper-Wiener index of the nanostar dendrimer G[n], defined
 *          pictorially in the Karbasioun-Ashrafi reference.
 * https://oeis.org/A221003
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = -6011 + 2^n*(11200*n^2 + 5920*n - 177792) +
 *            4^n*(102400*n^2 + 51200*n + 375936)
 * Range:     u8 none, u16 none, u32 n <= 5, u64 n <= 15, u128 n <= 15,
 *            big any n
 * Helpers:   pow2, pow4
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/pow2.h"
#include "../utils/pow4.h"

seq_t a(idx_t n)
{
    seq_t n_t = (seq_t)n;                 /* promote index to seq_t */
    seq_t n2  = n_t * n_t;                /* n^2  */

    seq_t pow2n = pow2(n);                /* 2^n  */
    seq_t pow4n = pow4(n);                /* 4^n  */

    /* positive contributions */
    seq_t pos2 = (seq_t)11200 * n2 + (seq_t)5920 * n_t;            /* 11200*n^2 + 5920*n */
    seq_t pos1 = (seq_t)102400 * n2 + (seq_t)51200 * n_t + (seq_t)375936; /* 102400*n^2 + 51200*n + 375936 */

    seq_t part_pos = pow4n * pos1 + pow2n * pos2;

    /* subtraction part */
    seq_t sub = pow2n * (seq_t)177792 + (seq_t)6011;

    return part_pos - sub;
}
