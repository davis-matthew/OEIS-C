/*
 * A222621  a(n) = (2n-1)^(2n).
 * https://oeis.org/A222621
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (2*n-1)^(2*n)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 5, u64 n <= 8, u128 n <= 13,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/big_pow.h"

/* Compute the OEIS sequence A222621 (offset 1):
   a(n) = (2*n-1)^(2*n)                                 */
seq_t a(idx_t n)
{
    /* n >= 1 is assumed, but the function works for n=0 as a(0)=1 */
    seq_t base = (seq_t)(2*n - 1);  /* base = 2n - 1 */
    seq_t exp  = (seq_t)(2*n);      /* exponent = 2n */
    return big_pow(base, exp);
}
