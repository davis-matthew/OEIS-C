/*
 * A019551  a(n) is the concatenation of n and 3n.
 * https://oeis.org/A019551
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, less, easy
 * Formula:   a(n) = n concatenated with 3n (decimal)
 * Range:     u8 n <= 3, u16 n <= 65, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   digit_len, pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/digit_len.h"
#include "../utils/pow10.h"

seq_t a(idx_t n)
{
    seq_t n_seq = (seq_t)n;          /* convert index to seq_t */
    seq_t t     = n_seq * 3;         /* compute 3n */
    int    td   = digit_len(t);      /* number of decimal digits of 3n */
    seq_t exp10 = pow10((idx_t)td); /* 10^td */
    return n_seq * exp10 + t;        /* concatenate n and 3n */
}
