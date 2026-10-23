/*
 * A139328  Sums of rows of the triangle in A139325.
 * https://oeis.org/A139328
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A000720(2*n^2 - 1) - 1 (i.e., pi(2*n^2-1) - 1)
 * Range:     u8 n <= 11, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   compute_pi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/compute_pi.h"

seq_t a(idx_t n)
{
    /* For n = 1 the formula would give -1, but the sequence defines a(1) = 0 */
    if (n == 1)
        return 0;

    seq_t n_seq = (seq_t)n;
    seq_t limit  = (seq_t)2 * n_seq * n_seq - 1;
    seq_t pi_val = compute_pi((idx_t)limit);
    return pi_val - 1;
}
