/*
 * A100846  Concatenate (1,n,n,1).
 * https://oeis.org/A100846
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = concatenation of the decimal representations of 1, n, n,
 *            and 1. Computed as 1*(10^d)^2 + n*10^d + n*10^d + 1, where d =
 *            number of decimal digits of n
 * Range:     u8 none, u16 n <= 9, u32 n <= 9999, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   digits_in_idx, pow10_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/digits_in_idx.h"
#include "../utils/pow10_seq.h"

seq_t a(idx_t n)
{
    idx_t d = digits_in_idx(n);
    seq_t pow10_d = pow10_seq(d);

    seq_t val = (seq_t)1;          /* start with leftmost 1 */
    val = val * pow10_d + (seq_t)n;/* append first n */
    val = val * pow10_d + (seq_t)n;/* append second n */
    val = val * 10 + (seq_t)1;     /* append final 1 */

    return val;
}
