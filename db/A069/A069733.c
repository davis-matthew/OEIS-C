/*
 * A069733  Number of divisors d of n such that d or n/d is odd. Number of
 *          non-orientable coverings of the Klein bottle with n lists.
 * https://oeis.org/A069733
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  mult, easy, nonn
 * Formula:   a(n) = number of divisors d of n such that d or n/d is odd.
 *            Equivalent to: if n is odd, a(n) = τ(n); if n is even, let m = n
 *            / 2^e be the odd part of n, then a(n) = 2 * τ(m)
 * Range:     u8 n <= 460, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   odd_divisor_count
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/odd_divisor_count.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0; /* undefined for n=0, but safe */
    /* Count trailing zeros to get exponent of 2 */
    uint64_t e = 0;
    uint64_t m = (uint64_t)n;
    while ((m & 1ULL) == 0ULL) {
        m >>= 1;
        ++e;
    }
    /* m is now the odd part of n */
    seq_t tau = odd_divisor_count(m);
    if (e == 0) {
        return tau;
    } else {
        return tau * 2;
    }
}
