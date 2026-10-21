/*
 * A072680  Difference between (least prime >= n) and (largest prime <= n).
 * https://oeis.org/A072680
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = leastPrime≥n – greatestPrime≤n (difference between the
 *            least prime ≥ n and the largest prime ≤ n)
 * Range:     u8 n <= 793, u16 n <= 9441, u32 n <= 9441, u64 n <= 9441,
 *            u128 n <= 9441, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/is_prime_big.h"

/* Return a(n) for n >= 2.
   For n < 2 the behaviour is undefined by OEIS; we return 0. */
seq_t a(idx_t n)
{
    if (n < 2u)          /* safety, not required by OEIS assumptions */
        return (seq_t)0;

    /* Search downward for the greatest prime ≤ n. */
    idx_t low = n;
    while (1) {
        if (is_prime_big((seq_t)low))
            break;
        low--;                /* n is ≥ 2 so low never underflows */
    }

    /* Search upward for the least prime ≥ n. */
    idx_t high = n;
    while (1) {
        if (is_prime_big((seq_t)high))
            break;
        high++;
    }

    return (seq_t)(high - low);
}
