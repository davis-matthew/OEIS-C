/*
 * A226264  Number of additive Z_2 Z_8 codes of a certain type (see
 *          Siap-Aydogdu for precise definition).
 * https://oeis.org/A226264
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 12 * (2^(n+1) - 1), n ≥ 1
 * Range:     u8 n <= 3, u16 n <= 11, u32 n <= 27, u64 n <= 59, u128 n <= 100,
 *            big any n
 * Helpers:   seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/seq_pow.h"

seq_t a(idx_t n)
{
    if (n < 1)            /* offset is 1, values for n<1 are undefined */
        return 0;
    /* Compute 2^(n+1) as a seq_t */
    seq_t val = seq_pow((seq_t)2, n + 1); /* 2^(n+1) */
    val -= (seq_t)1;                      /* 2^(n+1) - 1 */
    val *= (seq_t)12;                     /* 12 * (2^(n+1) - 1) */
    return val;
}
