/*
 * A008787  a(n) = (n + 6)^n.
 * https://oeis.org/A008787
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n + 6)^n
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 14, u128 n <= 25,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    /* Convert the index to seq_t before arithmetic to avoid
       intermediate overflow in fixed‑width builds. */
    seq_t base = (seq_t)(n + 6);
    seq_t exp  = (seq_t)n;
    return big_pow(base, exp);
}
