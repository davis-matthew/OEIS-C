/*
 * A224432  The hyper-Wiener index of the dendrimer D_2[n], defined
 *          pictorially in the A. R. Ashrafi et al. reference.
 * https://oeis.org/A224432
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = -2557 - 76735*2^n - 9768*2^n*n - 44640*4^n*n + 79914*4^n
 *            + 14400*4^n*n^2 + 2640*2^n*n^2
 * Range:     u8 none, u16 n <= 1, u32 n <= 6, u64 n <= 17, u128 n <= 17,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    seq_t two   = big_pow((seq_t)2, (seq_t)n);   /* 2^n   */
    seq_t four  = big_pow((seq_t)4, (seq_t)n);   /* 4^n   */

    seq_t n_seq = (seq_t)n;
    seq_t n_sq  = n_seq * n_seq;

    /* positive part: 4^n*(79914 + 14400*n^2) + 2^n*2640*n^2 */
    seq_t pos_total = four * (seq_t)79914
                    + four * (seq_t)14400 * n_sq
                    + two  * (seq_t)2640  * n_sq;

    /* negative part: 2^n*(76735 + 9768*n) + 4^n*44640*n + 2557 */
    seq_t neg_total = two  * (seq_t)76735
                    + two  * (seq_t)9768  * n_seq
                    + four * (seq_t)44640 * n_seq
                    + (seq_t)2557;

    return pos_total - neg_total;
}
