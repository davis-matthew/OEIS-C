/*
 * A220528  a(n) = n^7 + 7*n + 7^n.
 * https://oeis.org/A220528
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^7 + 7*n + 7^n
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 11, u64 n <= 21, u128 n <= 21,
 *            big any n
 * Helpers:   seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/seq_pow.h"

seq_t a(idx_t n)
{
    seq_t sn       = (seq_t)n;              /* n as seq_t */
    seq_t n7       = seq_pow(sn, 7);        /* n^7 */
    seq_t seven_n  = seq_pow((seq_t)7, n);  /* 7^n */
    seq_t res      = n7 + seven_n + sn * (seq_t)7; /* n^7 + 7^n + 7*n */
    return res;
}
