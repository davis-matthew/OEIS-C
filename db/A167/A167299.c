/*
 * A167299  Totally multiplicative sequence with a(p) = 7*(p-2) for prime p.
 * https://oeis.org/A167299
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(1)=1; a(n)=0 for even n>1; for odd n, a(n)=∏_{p^e∥n}
 *            (7·(p−2))^e
 * Range:     u8 n <= 24, u16 n <= 524, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/seq_pow.h"

seq_t a(idx_t n)
{
    /* handle the first term explicitly */
    if (n == 1)
        return (seq_t)1;

    /* all even numbers except 1 give 0 because 7·(2−2)=0 */
    if ((n & 1) == 0)
        return (seq_t)0;

    seq_t result = (seq_t)1;
    uint64_t m = (uint64_t)n;

    /* factorise the odd number m */
    for (uint64_t p = 3; p <= m / p; p += 2) {
        if (m % p == 0) {
            int e = 0;
            while (m % p == 0) {
                m /= p;
                ++e;
            }
            seq_t base = (seq_t)7 * (seq_t)(p - 2);
            seq_t pw   = seq_pow(base, (idx_t)e);
            result *= pw;
        }
    }

    /* remaining prime factor, if any */
    if (m > 1) {
        seq_t base = (seq_t)7 * (seq_t)(m - 2);
        seq_t pw   = seq_pow(base, (idx_t)1);
        result *= pw;
    }

    return result;
}
