/*
 * A167607  Sum of cousin prime pairs.
 * https://oeis.org/A167607
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum of the n‑th pair of cousin primes (primes that differ
 *            by 4)
 * Range:     u8 n <= 11, u16 n <= 501, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    if (n == 0)               /* undefined value, guard */
        return (seq_t)0;

    idx_t count = 0;          /* number of cousin‑prime pairs found */
    idx_t p = 3;              /* start checking from the first odd prime */

    while (1) {
        if (is_prime(p) && is_prime(p + 4)) {
            ++count;
            if (count == n)
                return (seq_t)(p + p + 4);   /* sum of the pair */
        }
        p += 2;               /* skip even numbers after 2 */
    }
}
