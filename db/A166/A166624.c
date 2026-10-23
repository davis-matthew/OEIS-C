/*
 * A166624  Totally multiplicative sequence with a(p) = 3p for prime p.
 * https://oeis.org/A166624
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = n * 3^Ω(n) (Ω(n) = total number of prime factors of n,
 *            counted with multiplicity)
 * Range:     u8 n <= 11, u16 n <= 95, u32 n <= 8191, u64 n <= 10000,
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
    if (n == 0)
        return 0;                      /* offset starts at 1; safety for n == 0 */

    seq_t omega = bigomega(n);          /* Ω(n) */
    seq_t pow3   = big_pow((seq_t)3, omega); /* 3^Ω(n) */

    return (seq_t)n * pow3;             /* a(n) = n * 3^Ω(n) */
}
