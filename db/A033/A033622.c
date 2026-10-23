/*
 * A033622  Good sequence of increments for Shell sort (best on big values).
 * https://oeis.org/A033622
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 9*2^n - 9*2^(n/2) + 1 when n is even, * a(n) = 8*2^n -
 *            6*2^((n+1)/2) + 1 when n is odd
 * Range:     u8 n <= 5, u16 n <= 13, u32 n <= 29, u64 n <= 61, u128 n <= 125,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if ((n & 1) == 0) {                      /* n is even */
        seq_t two_n      = pow2(n);
        seq_t two_half   = pow2(n / 2);
        return ((seq_t)9 * two_n) - ((seq_t)9 * two_half) + (seq_t)1;
    } else {                                 /* n is odd */
        seq_t two_n      = pow2(n);
        seq_t two_half   = pow2((n + 1) / 2);
        return ((seq_t)8 * two_n) - ((seq_t)6 * two_half) + (seq_t)1;
    }
}
