/*
 * A219642  Number of steps to reach 0 starting with n and using the iterated
 *          process: x -> x - (number of 1's in Zeckendorf expansion of x).
 * https://oeis.org/A219642
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 0; for n > 0, a(n) = 1 + a(n - z(n)), where z(n) is the
 *            number of terms in the Zeckendorf expansion of n
 * Range:     u8 n <= 382, u16 n <= 9367, u32 n <= 9367, u64 n <= 9367,
 *            u128 n <= 9367, big any n
 * Helpers:   zeckendorf_count
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/zeckendorf_count.h"

/* OEIS sequence A219642. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;

    seq_t steps = 0;
    seq_t m = (seq_t)n;   /* work in seq_t to avoid conversion on loop */

    while (m > 0) {
        seq_t t = zeckendorf_count(m);
        m -= t;
        ++steps;
    }

    return steps;
}
