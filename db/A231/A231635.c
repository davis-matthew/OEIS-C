/*
 * A231635  Number of ways to write n = x + y with 0 < x <= y such that lcm(x,
 *          y) + 1 is prime.
 * https://oeis.org/A231635
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = |{(x,y) : 0 < x ≤ y, x + y = n, lcm(x,y)+1 is prime}|
 * Range:     u8 n <= 32, u16 n <= 1007, u32 n <= 1007, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   gcd, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/gcd.h"
#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    seq_t cnt = 0;
    if (n <= 1) return 0;          /* n = 1 gives a(1) = 0 */
    idx_t half = n / 2;             /* largest possible x */
    for (idx_t x = 1; x <= half; ++x) {
        idx_t y = n - x;                  /* y ≥ x automatically */
        seq_t g  = gcd((seq_t)x, (seq_t)y);
        seq_t lcm = ((seq_t)x / g) * (seq_t)y;
        seq_t val = lcm + 1;
        if (is_prime_big(val))
            ++cnt;
    }
    return cnt;
}
