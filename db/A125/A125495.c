/*
 * A125495  Composite odious numbers.
 * https://oeis.org/A125495
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 89, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implements A125495 by searching composite numbers whose binary
 * representation has an odd number of 1's (odious) and that are not prime. */
#include <stdbool.h>
#include <stdint.h>
#include "../utils/is_prime.h"

/* Computes the binary popcount (number of 1 bits) of a 64‑bit value. */
static unsigned popcount_ull(uint64_t x)
{
    unsigned c = 0;
    while (x) {
        c += (unsigned)(x & 1U);
        x >>= 1;
    }
    return c;
}

seq_t a(idx_t n)
{
    /* n is 1‑based; the first term is 4. */
    if (n == 0) return 0;          /* safety, offset is 1 */
    idx_t count  = 0;
    seq_t num    = 4;               /* first composite odious candidate */

    while (1) {
        idx_t num64 = (idx_t)num;   /* guarantee we stay within 64‑bit limits */
        /* Test for composite: not prime and > 1. */
        if (!is_prime(num64) && num64 > 1) {
            /* Popcount must be odd => odd number of 1 bits. */
            if ((popcount_ull((uint64_t)num64) & 1U) != 0) {
                ++count;
                if (count == n)
                    return num;
            }
        }
        ++num;
    }
}
