/*
 * A073940  Number of n X n matrices over an alphabet of size 14.
 * https://oeis.org/A073940
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 14^(n^2)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 2, u64 n <= 4, u128 n <= 5,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    /* Convert index to seq_t before arithmetic */
    seq_t exp = (seq_t)n * (seq_t)n;
    seq_t base = (seq_t)14;
    return big_pow(base, exp);
}
