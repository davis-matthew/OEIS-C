/*
 * A167626  Prime numbers ending in the prime number 101.
 * https://oeis.org/A167626
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = 1000*A105106(n)+101 Sequence A167626: Prime numbers
 *            ending in 101
 * Range:     u8 n <= 1, u16 n <= 11, u32 n <= 17, u64 n <= 2000,
 *            u128 n <= 2000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    /* n is 1‑based; ignore n<1 silently */
    if (n < 1) return (seq_t)0;

    idx_t found = 0;          /* count of found terms */
    idx_t k     = 0;          /* current k in 1000*k+101 */

    while (true)
    {
        seq_t candidate = (seq_t)1000 * (seq_t)k + (seq_t)101;
        if (is_prime_big(candidate))
        {
            found++;
            if (found == n)
                return candidate;
        }
        k++;  /* move to next k */
    }
}
