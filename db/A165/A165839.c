/*
 * A165839  Totally multiplicative sequence with a(p) = 18.
 * https://oeis.org/A165839
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = 18^bigomega(n)
 * Range:     u8 n <= 3, u16 n <= 15, u32 n <= 255, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   big_pow, bigomega
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/bigomega.h"
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    /* a(1) = 1, a(p) = 18 for prime p, a(n) = 18^ω(n) for any n ≥ 1 */
    if (n == (idx_t)0)
        return (seq_t)0;          /* index 0 is outside the sequence */
    seq_t omega = bigomega(n);
    return big_pow((seq_t)18, omega);
}
