/*
 * A092089  Number of odd-length palindromes among the k-tuples of partial
 *          quotients of the continued fraction expansions of n/r, r = 1, ...,
 *          n.
 * https://oeis.org/A092089
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(1) = 1. For n > 1 let n = 2^e * Π p_i^{e_i} (p_i odd primes).
 *            Then a(n) = (e==0?1:(e==1?2:4*(e-1))) * Π (2*e_i + 1). This uses
 *            the known multiplicative formula for A092089
 * Range:     u8 n <= 1679, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* handle the offset */
    if (n == 0) return (seq_t)0;          /* undefined, but safe */
    if (n == 1) return (seq_t)1;

    uint64_t m = (uint64_t)n;        /* work in native unsigned for factorisation */
    seq_t result = (seq_t)1;

    /* factor out the power of 2 */
    if ((m & 1) == 0) {
        uint64_t e = 0;
        while ((m & 1) == 0) {
            m >>= 1;
            ++e;
        }
        if (e == 1)          result *= (seq_t)2;
        else if (e > 1)      result *= (seq_t)(4 * (e - 1));
        /* e==0 cannot happen here */
    }

    /* factor out odd primes by simple trial division */
    for (uint64_t p = 3; p <= m / p; p += 2) {
        if (m % p == 0) {
            uint64_t e = 0;
            while (m % p == 0) {
                m /= p;
                ++e;
            }
            result *= (seq_t)(2 * e + 1);
        }
    }

    /* any remaining prime factor > 1 has exponent 1 */
    if (m > 1) {
        result *= (seq_t)3;       /* 2*1 + 1 */
    }

    return result;
}
