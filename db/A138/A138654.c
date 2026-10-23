/*
 * A138654  7^n mod 4^n.
 * https://oeis.org/A138654
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 7^n mod 4^n
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   mod_pow, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/pow2.h"
#include "../utils/mod_pow.h"

seq_t a(idx_t n)
{
    /* 4^n = 2^(2n) */
    idx_t exp2 = n << 1;
    seq_t mod = pow2(exp2);
    seq_t base = (seq_t)7;
    return mod_pow(base, n, mod);
}
