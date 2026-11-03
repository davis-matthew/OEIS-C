/*
 * A212793  Characteristic function of cubefree numbers, A004709.
 * https://oeis.org/A212793
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = 1 if n is cubefree (no prime cube divides n), otherwise 0
 * Range:     u8 n <= 100000, u16 n <= 100000, u32 n <= 100000,
 *            u64 n <= 100000, u128 n <= 100000, big any n
 * Validated: standard, pass, 121 terms through n = 9403 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* We only need to detect if any prime divides n at least three
       times.  Any prime with exponent ≥3 must be ≤ ∛n, so we trial
       divide only up to the integer cube root of n. */
    uint64_t m = (uint64_t)n;        /* index fits in uint64_t */
    for (uint64_t p = 2; p * p * p <= m; ++p) {
        int exp = 0;
        while (m % p == 0) {
            m /= p;
            ++exp;
            if (exp >= 3) {
                return (seq_t)0;
            }
        }
    }
    /* After removing all primes ≤ ∛n, the remaining factor m (if >1)
       is either prime or a product of two distinct primes or a prime
       squared.  In any case its exponent is ≤2, so a(n) = 1. */
    return (seq_t)1;
}
