/*
 * A130107  Möbius transform of A063659.
 * https://oeis.org/A130107
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = product over prime powers p^e ∥ n of a(p^e) = { p-1 if
 *            e==1 p^2 - p - 1 if e==2 p^(e-3)*(p+1)*(p-1)^2 if e>=3 }
 * Range:     u8 n <= 256, u16 n <= 2000, u32 n <= 2000, u64 n <= 2000,
 *            u128 n <= 2000, big any n
 * Helpers:   prime_term
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/prime_term.h"

seq_t a(idx_t n)
{
    /* n >= 1 by definition */
    if (n == 1)
        return 1;

    seq_t result = 1;
    uint64_t m = (uint64_t)n;
    int e = 0;

    /* factor 2 */
    while ((m & 1) == 0) {
        ++e;
        m >>= 1;
    }
    if (e > 0)
        result *= prime_term(2, e);

    /* odd factors */
    for (uint64_t p = 3; p * p <= m; p += 2) {
        if (m % p == 0) {
            e = 0;
            while (m % p == 0) {
                ++e;
                m /= p;
            }
            result *= prime_term(p, e);
        }
    }

    /* remaining prime factor, if any */
    if (m > 1)
        result *= prime_term(m, 1);

    return result;
}
