/*
 * A123387  Number of triangular numbers <= n-th prime.
 * https://oeis.org/A123387
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = floor((sqrt(8*p_n+1)-1)/2) + 1 , where p_n is the n-th
 *            prime
 * Range:     u8 n <= 3503, u16 n <= 5000, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    /* Get the n-th prime (32‑bit). */
    uint32_t prime = get_prime(n);

    /* Work with a 64‑bit integer to avoid overflow. */
    uint64_t limit = (uint64_t)prime;

    uint64_t m = 0;
    while ((m + 1) * (m + 2) / 2 <= limit) {
        ++m;
    }

    /* Convert back to seq_t; if seq_t is smaller than m+1, the
       overflow is acceptable per the specification. */
    return (seq_t)m + (seq_t)1;
}
