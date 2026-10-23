/*
 * A138435  a(n) = ((n-th prime)^5-(n-th prime)^3)/6.
 * https://oeis.org/A138435
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ((p_n)^5 - (p_n)^3) / 6 where p_n is the n-th prime
 *            (offset 1)
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 30, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;          /* Undefined for offset 0, but harmless */

    /* Retrieve the n‑th prime.  get_prime returns a uint32_t. */
    uint32_t p = (uint32_t)get_prime(n);

    /* Do the arithmetic in an unsigned 128‑bit type to avoid overflow. */
    unsigned __int128 p128 = (unsigned __int128)p;
    unsigned __int128 p3   = p128 * p128 * p128;     /* p^3   */
    unsigned __int128 p5   = p3 * p128 * p128;       /* p^5   */
    unsigned __int128 diff = p5 - p3;
    unsigned __int128 res  = diff / 6;

    return (seq_t)res;
}
