/*
 * A035187  Sum over divisors d of n of Kronecker symbol (5|d).
 * https://oeis.org/A035187
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) = Σ_{d|n} Kronecker(5,d) where Kronecker(5,d) is the Jacobi
 *            symbol (5|d)
 * Range:     u8 n <= 5000, u16 n <= 5000, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n == 0)                /* offset is 1; return 0 for n<1 if ever called */
        return 0;

    seq_t result = 1;
    idx_t x = n;

    for (idx_t p = 2; p * p <= x; ++p) {
        if (x % p == 0) {
            idx_t exp = 0;
            while (x % p == 0) {
                x /= p;
                ++exp;
            }

            seq_t mul;
            if (p == 5) {
                mul = 1;                  /* a(5ᵉ) = 1 */
            } else {
                uint64_t mod = (uint64_t)p % 5;
                if (mod == 1 || mod == 4) {
                    mul = (seq_t)(exp + 1);    /* a(pᵉ) = e+1 for p≡1,4 (mod5) */
                } else { /* p≡2 or 3 (mod5) */
                    mul = (exp % 2 == 0) ? 1 : 0; /* a(pᵉ) = 1 if e even, else 0 */
                }
            }

            result *= mul;
            if (result == 0)                /* early exit if product becomes zero */
                return 0;
        }
    }

    /* remaining prime factor > sqrt(original n) */
    if (x > 1) {
        seq_t mul;
        if (x == 5) {
            mul = 1;
        } else {
            uint64_t mod = (uint64_t)x % 5;
            if (mod == 1 || mod == 4) {
                mul = 2;      /* exponent 1 -> e+1 = 2 */
            } else {         /* mod 2 or 3 -> odd exponent -> 0 */
                mul = 0;
            }
        }
        result *= mul;
    }

    return result;
}
