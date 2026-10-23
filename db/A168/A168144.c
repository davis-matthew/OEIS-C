/*
 * A168144  First differences of A168143 which are different from 1,
 *          incremented by 14.
 * https://oeis.org/A168144
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2^(n+1) + 15
 * Range:     u8 n <= 5, u16 n <= 5, u32 n <= 5, u64 n <= 5, u128 n <= 5,
 *            big any n
 * Helpers:   seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the n‑th term of OEIS A168144.  The sequence starts with
 * a(1) = 19 and follows a(n) = 2^(n+1) + 15 for all n ≥ 1.  */
#include "../utils/seq_pow.h"

seq_t a(idx_t n)
{
    /* exponent is n+1 (idx_t addition).  seq_pow handles large
     * intermediate values efficiently by repeated squaring. */
    seq_t value = seq_pow((seq_t)2, n + (idx_t)1);
    value += (seq_t)15;
    return value;
}
