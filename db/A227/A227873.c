/*
 * A227873  Sum of odious divisors of n. See A000069 for odious numbers.
 * https://oeis.org/A227873
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = Sum_{d|n, popcount(d) odd} d (sum of odious divisors of
 *            n). A divisor is odious when the number of 1‑bits in its binary
 *            representation is odd. The function enumerates all divisors of n
 *            and adds the odious ones. Complexity O(sqrt(n))
 * Range:     u8 n <= 139, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* check whether the integer d has odd popcount */
static bool is_odious(idx_t d)
{
    unsigned long long x = d;        /* d <= n <= 10000, fits in 64 bits */
    bool parity = false;
    while (x) {
        parity ^= 1;                 /* toggle parity for each set bit */
        x &= x - 1;                  /* clear least‑significant set bit */
    }
    return parity;
}

seq_t a(idx_t n)
{
    seq_t sum = 0;
    for (idx_t i = 1; i * i <= n; ++i) {
        if (n % i == 0) {
            if (is_odious(i))
                sum += (seq_t)i;
            idx_t j = n / i;
            if (j != i && is_odious(j))
                sum += (seq_t)j;
        }
    }
    return sum;
}
