/*
 * A066608  a(n) = 4^n mod n^4.
 * https://oeis.org/A066608
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 4^n mod n^4
 * Range:     u8 n <= 3, u16 n <= 15, u32 n <= 259, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/mod_pow.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0; /* behavior for n < 1 is irrelevant */
    seq_t nn = (seq_t)n;
    seq_t n2 = nn * nn;
    seq_t mod = n2 * n2; /* n^4 */
    return mod_pow((seq_t)4, n, mod);
}
