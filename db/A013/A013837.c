/*
 * A013837  a(n) = 5^(5*n + 4).
 * https://oeis.org/A013837
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 5^(5*n + 4)
 * Range:     u8 none, u16 n <= 0, u32 n <= 1, u64 n <= 4, u128 n <= 10,
 *            big any n
 * Helpers:   pow_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/pow_seq.h"

/* Computes 5^(5*n + 4) for any n >= 0. */
seq_t a(idx_t n)
{
    /* Exponent fits in an int for the intended range (n up to about 10000). */
    int exp = (int)(5 * n + 4);
    return pow_seq((seq_t)5, exp);
}
