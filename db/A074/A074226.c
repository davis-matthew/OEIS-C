/*
 * A074226  Numbers n such that Kronecker(3,n) = 1.
 * https://oeis.org/A074226
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-th positive integer k such that Kronecker(3,k) = 1
 * Range:     u8 n <= 88, u16 n <= 1666, u32 n <= 1666, u64 n <= 1666,
 *            u128 n <= 1666, big any n
 * Validated: standard, pass, 76 terms through n = 1392 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* Kronecker symbol (3 / n) for n >= 1.
   Returns 1 if (3/n)=1, -1 if (3/n)=-1, 0 if (3/n)=0. */
static int kronecker3_uint64(uint64_t n)
{
    if (n % 3ULL == 0ULL)        /* (3/3)=0, and any multiple of 3 gives 0 */
        return 0;

    int sign = 1;

    /* powers of 2: (3/2) = -1 */
    int e2 = 0;
    while ((n & 1ULL) == 0ULL) {
        n >>= 1ULL;
        ++e2;
    }
    if (e2 & 1)                 /* odd exponent of 2 */
        sign = -sign;

    /* factor remaining odd part */
    for (uint64_t p = 3ULL; p * p <= n; p += 2ULL) {
        if (n % p == 0ULL) {
            int exp = 0;
            while (n % p == 0ULL) {
                n /= p;
                ++exp;
            }
            if (exp & 1) {       /* only odd exponents affect the product */
                /* Legendre symbol (3/p) = 1 if p ≡ ±1 (mod 12), else -1 */
                int r = p % 12ULL;
                int leg = (r == 1 || r == 11) ? 1 : -1;
                sign *= leg;
            }
        }
    }

    if (n > 1ULL) {              /* remaining odd prime factor */
        /* n is now a prime > sqrt(original n), so its exponent is 1 (odd) */
        int r = n % 12ULL;
        int leg = (r == 1 || r == 11) ? 1 : -1;
        sign *= leg;
    }

    return sign;
}

seq_t a(idx_t n)
{
    idx_t found = 0;
    uint64_t candidate = 1ULL;

    while (1) {
        if (kronecker3_uint64(candidate) == 1)
            ++found;
        if (found == n)
            return (seq_t)candidate;
        ++candidate;
    }
}
