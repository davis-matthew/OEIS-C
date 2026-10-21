/*
 * A228429  Number of ways to write n = x + y (x, y > 0) with p(39, x) + p(40,
 *          y) prime, where p(m, k) denotes the m-gonal number (m-2)*k*(k-1)/2
 *          + k.
 * https://oeis.org/A228429
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 52, u32 n <= 60, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime_big, polygon
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = number of ordered pairs (x, y) with x > 0, y > 0,
 *                x + y = n, such that p(39, x) + p(40, y) is prime.
 *   p(m, k) = (m-2)·k·(k-1)/2 + k
 * This implementation counts all such pairs by brute force.
 * It uses the generic prime test `is_prime_big` from the utils set.
 */

#include <stdbool.h>
#include "../utils/is_prime_big.h"
#include "../utils/polygon.h"

seq_t a(idx_t n)
{
    if (n < 2)
        return 0;

    seq_t cnt = 0;
    for (idx_t x = 1; x < n; ++x)
    {
        idx_t y = n - x;
        seq_t px = polygon(39, x);
        seq_t py = polygon(40, y);
        seq_t sum = px + py;
        if (is_prime_big(sum))
            ++cnt;
    }
    return cnt;
}
