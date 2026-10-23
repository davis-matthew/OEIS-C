/*
 * A138955  a(n) = 8^n mod 3^n.
 * https://oeis.org/A138955
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 8^n mod 3^n
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 20, u64 n <= 28, u128 n <= 28,
 *            big any n
 * Helpers:   mod_pow, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/seq_pow.h"
#include "../utils/mod_pow.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    seq_t mod = seq_pow((seq_t)3, n);
    return mod_pow((seq_t)8, n, mod);
}
