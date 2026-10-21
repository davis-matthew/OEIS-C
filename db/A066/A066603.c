/*
 * A066603  a(n) = 5^n mod n.
 * https://oeis.org/A066603
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 5^n mod n
 * Range:     u8 n <= 314, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   _mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/_mod_pow.h"

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;         /* undefined for n=0, but return 0 */
    return _mod_pow((seq_t)5, n, (seq_t)n);
}
