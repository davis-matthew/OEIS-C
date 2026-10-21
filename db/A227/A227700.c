/*
 * A227700  The hyper-Wiener index of the nanostar dendrimer defined
 *          pictorially in the Ashrafi et al. reference.
 * https://oeis.org/A227700
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 16351 + 2^n*(26400*n^2 + 52080*n + 40656) +
 *            4^n*(57600*n^2 + 42240*n + 242664)
 * Range:     u8 none, u16 none, u32 n <= 5, u64 n <= 17, u128 n <= 17,
 *            big any n
 * Helpers:   pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/pow_uint.h"

seq_t a(idx_t n)
{
    seq_t n_seq = (seq_t)n;
    seq_t n2 = n_seq * n_seq;

    seq_t coef2 = (seq_t)26400 * n2 + (seq_t)52080 * n_seq + (seq_t)40656;
    seq_t coef3 = (seq_t)57600 * n2 + (seq_t)42240 * n_seq + (seq_t)242664;

    seq_t p2 = pow_uint(2, n);
    seq_t p4 = pow_uint(4, n);

    seq_t term1 = (seq_t)16351;
    seq_t term2 = p2 * coef2;
    seq_t term3 = p4 * coef3;

    return term1 + term2 + term3;
}
