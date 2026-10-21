/*
 * A220455  Number of ways to write n=x+y (x>0, y>0) with 3x-2, 3x+2 and 2xy+1
 *          all prime.
 * https://oeis.org/A220455
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of pairs x>0 , y>0 with n = x + y such that *
 *            3x-2, 3x+2 and 2xy+1 are all prime
 * Range:     u8 n <= 22, u16 n <= 541, u32 n <= 541, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    if (n < 2) return 0;
    seq_t count = 0;
    for (idx_t x = 1; x < n; ++x) {
        idx_t y = n - x;
        seq_t sx = (seq_t)x;
        seq_t sy = (seq_t)y;

        seq_t v1 = 3 * sx - 2;
        if (!is_prime_big(v1)) continue;

        seq_t v2 = 3 * sx + 2;
        if (!is_prime_big(v2)) continue;

        seq_t v3 = 2 * sx * sy + 1;
        if (!is_prime_big(v3)) continue;

        ++count;
    }
    return count;
}
