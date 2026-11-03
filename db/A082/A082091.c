/*
 * A082091  a(n) = one more than the number of iterations of A005361 needed to
 *          reach 1 from the starting value n.
 * https://oeis.org/A082091
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 + a(A005361(n)), a(1) = 1 A005361(n) = product of the
 *            exponents in the prime factorisation of n
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 122 terms through n = 9563 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* product of exponents in the prime factorisation of n (n > 1) */
static inline seq_t prod_exp(uint64_t n)
{
    seq_t prod = 1;        /* start with multiplicative identity */
    seq_t exp  = 0;

    /* factor 2 */
    while ((n & 1ULL) == 0) {
        n >>= 1;
        ++exp;
    }
    if (exp > 0)
        prod *= exp;

    /* odd prime factors */
    for (uint64_t p = 3; p * p <= n; p += 2) {
        exp = 0;
        while (n % p == 0) {
            n /= p;
            ++exp;
        }
        if (exp > 0)
            prod *= exp;
    }

    /* remaining prime (exponent 1) does not change the product */
    return prod;
}

seq_t a(idx_t n)
{
    idx_t x = n;
    seq_t steps = 0;

    while (x != 1) {
        seq_t f = prod_exp((uint64_t)x);
        x = (idx_t)f;   /* f will fit in idx_t for the intended ranges */
        ++steps;
    }

    return steps + 1;
}
