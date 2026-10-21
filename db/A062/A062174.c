/*
 * A062174  a(n) = 3^(n-1) mod n.
 * https://oeis.org/A062174
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3^(n-1) mod n
 * Range:     u8 n <= 298, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/mod_pow.h"

seq_t a(idx_t n)
{
    /* n is guaranteed >= 1 by the OEIS offset */
    idx_t exp = n - 1;
    seq_t base = (seq_t)3;
    seq_t mod  = (seq_t)n;
    return mod_pow(base, exp, mod);
}
