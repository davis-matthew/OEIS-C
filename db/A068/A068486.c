/*
 * A068486  Smallest prime equal to n^2 + m^2 with n >= m.
 * https://oeis.org/A068486
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n^2 + min{m <= n | n^2 + m^2 is prime} The function
 *            searches for the smallest m in [0, n] that makes n^2 + m^2 prime
 * Range:     u8 n <= 15, u16 n <= 254, u32 n <= 302, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    /* n is at least 1 for the OEIS sequence; return 0 for n==0 */
    if (n == 0) return 0;

    seq_t nn = (seq_t)n * (seq_t)n;          /* n^2 */
    for (idx_t m = 0; m <= n; ++m) {
        seq_t mm = (seq_t)m * (seq_t)m;      /* m^2 */
        seq_t candidate = nn + mm;           /* n^2 + m^2 */
        if (is_prime_big(candidate))
            return candidate;
    }
    return 0;  /* should never happen */
}
