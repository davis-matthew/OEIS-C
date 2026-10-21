/*
 * A083528  a(n) = 5^n mod 2*n.
 * https://oeis.org/A083528
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 5^n mod (2*n)
 * Range:     u8 n <= 134, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   mod_pow_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/mod_pow_seq.h"

seq_t a(idx_t n)
{
    seq_t base = (seq_t)5;
    seq_t mod  = (seq_t)2 * (seq_t)n;
    return mod_pow_seq(base, (uint64_t)n, mod);
}
