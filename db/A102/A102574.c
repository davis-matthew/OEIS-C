/*
 * A102574  a(n) is the sum of the distinct norms of the divisors of n over
 *          the Gaussian integers.
 * https://oeis.org/A102574
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult, easy
 * Formula:   a(n) = Σ_{d | n², d is a sum of two squares} d The function
 *            below implements this multiplicatively
 * Range:     u8 n <= 11, u16 n <= 169, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   sum_all_powers, sum_even_powers
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/sum_all_powers.h"
#include "../utils/sum_even_powers.h"

seq_t a(idx_t n)
{
    if (n <= 1) return (seq_t)n;   /* a(1)=1, a(0)=0 (unused) */

    uint64_t m = (uint64_t)n;
    seq_t result = 1;

    /* handle factor 2 */
    if ((m & 1ULL) == 0) {
        uint32_t e = 0;
        while ((m & 1ULL) == 0) {
            ++e;
            m >>= 1;
        }
        result *= sum_all_powers(2, e);
    }

    /* odd prime factors */
    for (uint64_t p = 3; (unsigned __int128)p * p <= m; p += 2) {
        if (m % p == 0) {
            uint32_t e = 0;
            while (m % p == 0) {
                ++e;
                m /= p;
            }
            if (p % 4 == 3)
                result *= sum_even_powers(p, e);
            else
                result *= sum_all_powers(p, e);
        }
    }

    /* any remaining prime factor > sqrt(n) */
    if (m > 1) {
        uint64_t p = m;   /* exponent is 1 */
        if (p % 4 == 3)
            result *= sum_even_powers(p, 1);
        else
            result *= sum_all_powers(p, 1);
    }

    return result;
}
