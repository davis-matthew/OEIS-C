/*
 * A031915  a(n) = prime(8*n - 7).
 * https://oeis.org/A031915
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = prime(8*n - 7)
 * Range:     u8 n <= 7, u16 n <= 818, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    idx_t k = 8ULL * n - 7ULL;      /* index of the required prime (1‑based) */
    uint32_t p = get_prime(k);      /* get the k‑th prime number */
    return (seq_t)p;                /* return as seq_t */
}
