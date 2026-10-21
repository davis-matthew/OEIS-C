/*
 * A077808  Number of prime factors of numbers containing in their decimal
 *          representation only the digits 0 and 1 (counted with
 *          multiplicity).
 * https://oeis.org/A077808
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = A001222(A007088(n))
 * Range:     u8 n <= 7, u16 n <= 31, u32 n <= 1928, u64 n <= 8190,
 *            u128 n <= 8190, big any n
 * Helpers:   count_prime_factors
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/count_prime_factors.h"

seq_t a(idx_t n)
{
    /* A007088(n) is the decimal representation of the binary number n. */
    if (n == 0) return 0;  /* offset is 1; n>=1 in normal use */

    idx_t temp = n;
    /* Find the position of the most significant bit of n. */
    idx_t msb = 0;
    idx_t t = temp;
    while (t >>= 1) ++msb;

    /* Build the decimal number by parsing the bits from msb to lsb. */
    seq_t val = 0;
    for (idx_t i = msb; ; --i) {
        seq_t bit = (temp >> i) & 1;
        val = val * 10 + bit;
        if (i == 0) break;
    }

    /* Count prime factors with multiplicity (big‑omega). */
    return count_prime_factors(val);
}
