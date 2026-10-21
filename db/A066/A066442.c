/*
 * A066442  a(n) = 12^n mod n.
 * https://oeis.org/A066442
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 12^n mod n
 * Range:     u8 n <= 259, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/mod_pow.h"

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* out of offset; guard against division by zero */
    seq_t base = (seq_t)12;
    return mod_pow(base, n, (seq_t)n);
}
