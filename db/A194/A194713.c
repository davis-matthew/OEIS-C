/*
 * A194713  13 times hexagonal numbers: a(n) = 13*n*(2*n-1).
 * https://oeis.org/A194713
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 13*n*(2*n-1)
 * Range:     u8 n <= 3, u16 n <= 50, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Computes the nth term of OEIS A194713: 13 times the nth hexagonal number. */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;
    seq_t n_seq = (seq_t)n;
    seq_t two_n_minus1 = 2 * n_seq - 1;
    return 13 * n_seq * two_n_minus1;
}
