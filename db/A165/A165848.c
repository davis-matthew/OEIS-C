/*
 * A165848  Totally multiplicative sequence with a(p) = 27.
 * https://oeis.org/A165848
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = 27^A001222(n) = 27^bigomega(n)
 * Range:     u8 n <= 3, u16 n <= 15, u32 n <= 127, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   big_pow, bigomega
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/bigomega.h"
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    seq_t omega = bigomega(n);
    return big_pow((seq_t)27, omega);
}
