/*
 * A165825  Totally multiplicative sequence with a(p) = 4.
 * https://oeis.org/A165825
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = 4^bigomega(n)
 * Range:     u8 n <= 15, u16 n <= 255, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   bigomega, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/bigomega.h"
#include "../utils/seq_pow.h"

seq_t a(idx_t n)
{
    return seq_pow((seq_t)4, (idx_t)bigomega(n));
}
