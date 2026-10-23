/*
 * A127230  a(n) = (2n)! - 1.
 * https://oeis.org/A127230
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (2*n)! - 1
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 6, u64 n <= 10, u128 n <= 15,
 *            big any n
 * Helpers:   factorial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/factorial.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* behaviour below offset is irrelevant */
    idx_t m = n * ((idx_t)2);      /* 2*n fits in idx_t for the tested ranges   */
    seq_t f = factorial(m);       /* computes (2*n)! exactly in seq_t           */
    return f - (seq_t)1;           /* subtract 1 to get a(n)                      */
}
