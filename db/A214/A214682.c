/*
 * A214682  Remove 2's that do not contribute to a factor of 4 from the prime
 *          factorization of n.
 * https://oeis.org/A214682
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, mult
 * Formula:   a(n) = n / 2^(v2(n) mod 2) where v2(n) is the 2-adic valuation
 *            of n. Equivalent simplification: a(n) = n >> (v2(n) & 1)
 * Range:     u8 n <= 255, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    /* Compute v2(n) by counting trailing zeros. */
    uint64_t temp = (uint64_t)n;
    uint64_t v2 = 0;
    while ((temp & 1ULL) == 0) {
        ++v2;
        temp >>= 1;
    }
    /* Shift right by 0 if v2 is even, by 1 if v2 is odd. */
    uint64_t shift = v2 & 1ULL;
    return ((seq_t)n) >> (unsigned)shift;
}
