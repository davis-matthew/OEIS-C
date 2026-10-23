/*
 * A192006  a(n) = Sum_{d|n} Kronecker(-14, d) with a(0) = 2.
 * https://oeis.org/A192006
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) = Sum_{d|n} Kronecker(-14, d) with a(0) = 2. The sequence
 *            is multiplicative
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   jacobi_symbol
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/jacobi_symbol.h"

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)2;

    uint64_t m = (uint64_t)n;
    seq_t result = (seq_t)1;

    /* factor out 2 and 7 – for them the factor is 1, so we can ignore them */
    while ((m & 1ULL) == 0) m >>= 1;          /* divide by 2 */
    while (m % 7ULL == 0) m /= 7ULL;          /* divide by 7 */

    for (uint64_t p = 3; p * p <= m; p += 2) {
        if (m % p == 0) {
            uint64_t e = 0;
            while (m % p == 0) {
                m /= p;
                ++e;
            }
            /* p is odd and not 7, so Kronecker(-14,p) is ±1 */
            uint64_t mod = p - (14ULL % p);
            int s = jacobi_symbol(mod, p);   /* s = ±1 */
            if (s == 1) {
                result *= (seq_t)(e + 1);
            } else { /* s == -1 */
                if (e & 1ULL) {
                    result = (seq_t)0;
                    break;
                }
                /* e even => factor 1, nothing to multiply */
            }
        }
    }

    if (m > 1) {
        /* remaining prime factor m (exponent 1) */
        uint64_t p = m;
        /* p is odd and not 7 because we removed them earlier */
        uint64_t mod = p - (14ULL % p);
        int s = jacobi_symbol(mod, p);   /* ±1 */
        if (s == 1) {
            result *= (seq_t)(1 + 1);    /* e = 1  -> factor = 2 */
        } else { /* s == -1 */
            /* exponent is odd, so factor is 0 */
            result = (seq_t)0;
        }
    }

    return result;
}
