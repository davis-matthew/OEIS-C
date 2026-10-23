/*
 * A192013  a(n) = Sum_{d|n} Kronecker(-6, d).
 * https://oeis.org/A192013
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = Sum_{d|n} Kronecker(-6, d). Implements the multiplicative
 *            definition: a(1) = 1 ; a(2^e) = a(3^e) = 1 ; a(p^e) = e+1 if p ≡
 *            1,5,7,11 (mod 24); a(p^e) = (1+(-1)^e)/2 if p ≡ 13,17,19,23 (mod
 *            24)
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;          /* not used for OEIS offset but safe */

    seq_t result = 1;

    /* factor n by trial division */
    for (uint64_t p = 2; p * p <= (uint64_t)n; ++p) {
        if ((uint64_t)n % p == 0) {
            int exp = 0;
            uint64_t m = (uint64_t)n;
            while (m % p == 0) {
                m /= p;
                ++exp;
            }
            /* update n for the remainder */
            n = (idx_t)m;

            seq_t val = 1;
            if (p == 2 || p == 3) {
                /* a(2^e) = a(3^e) = 1 for all e */
                val = 1;
            } else {
                int r = (int)(p % 24ULL);
                if (r == 1 || r == 5 || r == 7 || r == 11) {
                    val = (seq_t)(exp + 1);
                } else { /* r == 13 || r == 17 || r == 19 || r == 23 */
                    val = (exp % 2 == 0) ? (seq_t)1 : (seq_t)0;
                }
            }
            result *= val;
            if (result == 0)
                return 0;   /* early exit */
        }
    }
    /* remaining prime factor, if any */
    if (n > 1) {
        uint64_t p = (uint64_t)n;
        int exp = 1;
        seq_t val = 1;
        if (p == 2 || p == 3) {
            val = 1;
        } else {
            int r = (int)(p % 24ULL);
            if (r == 1 || r == 5 || r == 7 || r == 11) {
                val = (seq_t)(exp + 1);
            } else { /* r == 13 || r == 17 || r == 19 || r == 23 */
                val = (exp % 2 == 0) ? (seq_t)1 : (seq_t)0;
            }
        }
        result *= val;
    }
    return result;
}
