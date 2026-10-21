/*
 * A213381  a(n) = n^n mod (n+2).
 * https://oeis.org/A213381
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n^n mod (n+2)
 * Range:     u8 n <= 261, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/mod_pow.h"

seq_t a(idx_t n)
{
    seq_t base = (seq_t)n;
    seq_t mod  = base + (seq_t)2;
    return mod_pow(base, (uint64_t)n, mod);
}
