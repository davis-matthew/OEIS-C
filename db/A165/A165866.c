/*
 * A165866  Totally multiplicative sequence with a(p) = 45.
 * https://oeis.org/A165866
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = 45^Ω(n) = 45^bigomega(n)
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 63, u64 n <= 4095,
 *            u128 n <= 10000, big any n
 * Helpers:   big_pow, bigomega
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/bigomega.h"
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    seq_t k = bigomega(n);            /* number of prime factors (with multiplicity) */
    return big_pow((seq_t)45, k);     /* 45^k */
}
