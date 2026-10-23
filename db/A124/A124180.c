/*
 * A124180  Prime(R(n)) where Prime(i) is the i-th prime and R(n) is the value
 *          of the reverse of the digits of n.
 * https://oeis.org/A124180
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, base
 * Formula:   a(n) = Prime(R(n))
 * Range:     u8 n <= 15, u16 n <= 61, u32 n <= 61, u64 n <= 61, u128 n <= 61,
 *            big any n
 * Helpers:   get_prime, rev
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/rev.h"          /* rev(idx_t) */
#include "../utils/get_prime.h"    /* uint32_t get_prime(idx_t) */

seq_t a(idx_t n)
{
    if (n < 1) return 0;
    idx_t r = rev(n);
    if (r == 0) return 0;
    /* get_prime returns the r‑th prime (1‑based).  It fits in uint32_t for the inputs of interest. */
    uint32_t p = get_prime(r);
    return (seq_t)p;
}
