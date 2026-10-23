/*
 * A175221  a(n) = prime(n) + 4.
 * https://oeis.org/A175221
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = prime(n) + 4
 * Range:     u8 n <= 54, u16 n <= 6542, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;           /* undefined for n < 1, behaviour irrelevant */
    uint32_t p = get_prime(n);      /* nth prime, 1‑based indexing */
    return (seq_t)p + (seq_t)4;     /* add 4 (exact accounting for seq_t) */
}
