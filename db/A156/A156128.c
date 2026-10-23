/*
 * A156128  a(n) = 6^n * Catalan(n).
 * https://oeis.org/A156128
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 6^n * Catalan(n)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 15, u128 n <= 29,
 *            big any n
 * Helpers:   big_pow, catalan
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"
#include "../utils/catalan.h"

seq_t a(idx_t n)
{
    seq_t pow6 = big_pow((seq_t)6, (seq_t)n);
    seq_t cat  = catalan(n);
    return pow6 * cat;
}
