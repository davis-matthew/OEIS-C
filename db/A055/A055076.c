/*
 * A055076  Multiplicity of Max{gcd(d, n/d)} when d runs over divisors of n.
 * https://oeis.org/A055076
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) = 2^{number of prime divisors of n having odd exponent}
 *            (multiplicative with a(p^e) = 2^(e mod 2), thus a(1)=1)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    if (n <= 1)               /* a(1) = 1 */
        return 1;

    uint64_t m = (uint64_t)n;
    unsigned int oddcnt = 0;

    /* Trial division up to sqrt(m) */
    for (uint64_t p = 2; p <= m / p; ++p) {
        if (m % p == 0) {
            unsigned int exp = 0;
            while (m % p == 0) {
                m /= p;
                ++exp;
            }
            if (exp & 1)          /* odd exponent contributes a factor 2 */
                ++oddcnt;
        }
    }

    if (m > 1)                  /* remaining prime factor with exponent 1 */
        ++oddcnt;

    seq_t res = 1;
    for (unsigned int i = 0; i < oddcnt; ++i)
        res = res * 2;          /* compute 2^oddcnt */

    return res;
}
