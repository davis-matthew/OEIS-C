/*
 * A162510  Dirichlet inverse of A076479.
 * https://oeis.org/A162510
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, mult, nonn
 * Formula:   a(n) = product_{p^e || n} 2^(e-1) (multiplicative; a(p^e) =
 *            2^(e-1) for every prime p and exponent e>=1)
 * Range:     u8 n <= 511, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Compute the range‑restricted multiplicative function A162510. */
seq_t a(idx_t n)
{
    if (n == 1)
        return (seq_t)1;          /* base case */

    /* Temporary variable for factorisation */
    uint64_t m = (uint64_t)n;
    idx_t    diff = 0;            /* exponent sum minus number of distinct primes */

    /* Handle factor 2 separately for speed */
    idx_t exp2 = 0;
    while ((m & 1ULL) == 0) {     /* while m is even */
        m >>= 1;                 /* divide by 2 */
        ++exp2;
    }
    if (exp2 > 0)
        diff += exp2 - 1;        /* 2^(exp2-1) contributes exp2-1 to exponent sum */

    /* Trial division for odd factors */
    for (uint64_t p = 3; p * p <= m; p += 2) {
        if (m % p == 0) {
            idx_t exp = 0;
            while (m % p == 0) {
                m /= p;
                ++exp;
            }
            diff += exp - 1;    /* each prime power contributes exp-1 */
        }
    }

    /* If m > 1 now, it is a prime factor with exponent 1 */
    if (m > 1)
        diff += 0;              /* exponent 1 contributes 0 */

    /* Compute 2^diff by repeated multiplication (avoiding pow2 helper) */
    seq_t result = 1;
    for (idx_t i = 0; i < diff; ++i)
        result *= 2;

    return result;
}
