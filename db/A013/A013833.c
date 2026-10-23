/*
 * A013833  a(n) = 4^(5*n + 4).
 * https://oeis.org/A013833
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4^(5*n + 4)
 * Range:     u8 none, u16 n <= 0, u32 n <= 2, u64 n <= 5, u128 n <= 11,
 *            big any n
 * Helpers:   seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0) = 256, a(n) = 1024 * a(n-1), n > 0 */

#include "../utils/seq_pow.h"

seq_t a(idx_t n)
{
    /* 4^(5*n+4) = 256 * 1024^n  */
    seq_t pow = seq_pow((seq_t)1024, n);   /* compute 1024^n */
    return pow * (seq_t)256;               /* multiply by 256 = 4^4  */
}
