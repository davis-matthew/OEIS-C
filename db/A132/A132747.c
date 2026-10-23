/*
 * A132747  a(n) = number of non-isolated divisors of n.
 * https://oeis.org/A132747
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of non‑isolated divisors of n A divisor d of n is
 *            non‑isolated if either d-1 or d+1 divides n
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   non_isolated
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/non_isolated.h"

seq_t a(idx_t n)
{
    if (n == 0)            /* out of definition range, defensive */
        return 0;
    if ((n & 1U) == 1)     /* all odd n have a(n)=0 */
        return 0;

    seq_t count = 0;

    /* compute floor(sqrt(n)) without using floating point */
    idx_t sq = 0;
    while ((sq + 1) <= n / (sq + 1))
        ++sq;

    for (idx_t i = 1; i <= sq; ++i) {
        if (n % i != 0)
            continue;

        /* divisor i */
        if (non_isolated(i, n))
            ++count;

        /* paired divisor n/i, avoid double counting perfect squares */
        idx_t d = n / i;
        if (d != i && non_isolated(d, n))
            ++count;
    }

    return count;
}
