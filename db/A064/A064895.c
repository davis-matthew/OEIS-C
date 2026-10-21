/*
 * A064895  Binary concentration of n. Replace 2^e_k with 2^(e_k/g(n)) in
 *          binary expansion of n, where g(n) = GCD of exponents e_k =
 *          A064894(n).
 * https://oeis.org/A064895
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = sum_{2^e|n} 2^(e / g(n)), where g(n)=GCD of the exponents
 *            e of the set bits in n
 * Range:     u8 n <= 257, u16 n <= 8192, u32 n <= 8192, u64 n <= 8192,
 *            u128 n <= 8192, big any n
 * Helpers:   gcd, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* n = 0 is the sequence offset */
    if (n == 0)
        return 0;

    /* 1. Compute g(n) = GCD of the set‑bit exponents of n */
    idx_t g = 0;
    idx_t pos = 0;
    idx_t tmp = n;
    while (tmp)
    {
        if (tmp & 1ULL)
        {
            if (g == 0)
                g = pos;
            else
                g = (idx_t)gcd((seq_t)g, (seq_t)pos);
        }
        tmp >>= 1;
        ++pos;
    }

    /* Special case: n = 1 (exponent set {0}) gives g = 0 */
    if (g == 0)
        return 1;

    /* 2. Build the result using the reduced exponents e/g */
    seq_t result = 0;
    pos = 0;
    tmp = n;
    while (tmp)
    {
        if (tmp & 1ULL)
        {
            idx_t e_div = pos / g;        /* integer division, g > 0 */
            result |= pow2(e_div);
        }
        tmp >>= 1;
        ++pos;
    }

    return result;
}
