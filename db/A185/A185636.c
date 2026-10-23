/*
 * A185636  a(n) = |{0 <= k < n: n+k and n+k^2 are both prime}|.
 * https://oeis.org/A185636
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4749, u16 n <= 9734, u32 n <= 9734, u64 n <= 9734,
 *            u128 n <= 9734, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence a(n): count of k, 0 <= k < n, such that n+k and n+k^2 are prime. */
#include <stdbool.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    idx_t count = 0;
    for (idx_t k = 0; k < n; ++k) {
        idx_t s1 = n + k;          /* n + k   */
        idx_t s2 = n + k * k;      /* n + k^2 */
        if (is_prime(s1) && is_prime(s2))
            ++count;
    }
    return (seq_t)count;
}
