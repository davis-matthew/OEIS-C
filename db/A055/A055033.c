/*
 * A055033  a(n) = usigma(usigma(n)), where usigma(n) is the sum of unitary
 *          divisors of n (A034448).
 * https://oeis.org/A055033
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = A034448(A034448(n)) where A034448(m) = sum of unitary
 *            divisors of m (product over prime powers (1 + p^e))
 * Range:     u8 n <= 77, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 77 terms through n = 9953 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Compute the unitary divisor sum of a uint64_t number.
   Assumes the result fits into a seq_t, which is always true for
   n <= 10000 (and all values used in tests). */
static seq_t unitary_sigma_uint64(uint64_t n)
{
    seq_t res = 1;          /* accumulate product of (1 + p^e) terms   */
    uint64_t x = n;
    for (uint64_t p = 2; p * p <= x; ++p) {
        if (x % p == 0) {
            int exp = 0;
            while (x % p == 0) {
                x /= p;
                ++exp;
            }
            seq_t pow = 1;               /* p^exp */
            for (int i = 0; i < exp; ++i)
                pow = pow * (seq_t)p;
            seq_t term = pow + 1;        /* (1 + p^exp) */
            res = res * term;
        }
    }
    if (x > 1) {                        /* remaining prime factor */
        seq_t term = (seq_t)x + 1;
        res = res * term;
    }
    return res;
}

seq_t a(idx_t n)
{
    /* First unitary sigma of n */
    uint64_t n64 = (uint64_t)n;
    seq_t s1 = unitary_sigma_uint64(n64);

    /* Since s1 will be small for all n used in tests, we safely cast
       it back to uint64_t for the second application. */
    uint64_t s1_64 = (uint64_t)s1;
    seq_t s2 = unitary_sigma_uint64(s1_64);

    return s2;
}
