/*
 * A061142  Replace each prime factor of n with 2: a(n) = 2^bigomega(n), where
 *          bigomega = A001222, number of prime factors counted with
 *          multiplicity.
 * https://oeis.org/A061142
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, mult
 * Formula:   a(n) = 2^Ω(n), where Ω(n) is the total number of prime factors
 *            of n counted with multiplicity (OEIS A001222)
 * Range:     u8 n <= 255, u16 n <= 9529, u32 n <= 9529, u64 n <= 9529,
 *            u128 n <= 9529, big any n
 * Helpers:   bigomega
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* We use the helper bigomega(idx_t n) to get Ω(n). */
#include "../utils/bigomega.h"   /* provides seq_t bigomega(idx_t n) */

seq_t a(idx_t n)
{
    /* Offset is 1: for n >= 1 the definition holds.
       For completeness handle n == 0 with the same value as a(1). */
    if (n == 0) return (seq_t)1;

    /* Compute the exponent Ω(n).  It fits easily in a 64‑bit integer. */
    uint64_t omega = (uint64_t)bigomega(n);

    /* Compute 2^omega by repeated left shift.  This works for all
       unsigned integer sizes; if seq_t is arbitrary precision the shift
       operator is defined for that type in the build. */
    seq_t result = 1;
    for (uint64_t i = 0; i < omega; ++i) {
        result <<= 1;
    }
    return result;
}
