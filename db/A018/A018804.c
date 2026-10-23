/*
 * A018804  Pillai's arithmetical function: Sum_{k=1..n} gcd(k, n).
 * https://oeis.org/A018804
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult, changed
 * Formula:   a(n) = Sum_{k=1..n} gcd(k, n)
 * Range:     u8 n <= 55, u16 n <= 3779, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    seq_t sum = 0;
    for (idx_t k = 1; k <= n; ++k)
    {
        sum += gcd((seq_t)k, (seq_t)n);
    }
    return sum;
}
