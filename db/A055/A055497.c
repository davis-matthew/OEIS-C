/*
 * A055497  a(-1) = 4, a(0) = 5; thereafter a(n) = 4 + (Product_{k=1..n}
 *          prime(k))^2.
 * https://oeis.org/A055497
 *
 * Offset:    -1    (a(-1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 4 + (∏_{k=1..n} prime(k))² with a(-1) = 4, a(0) = 5
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 6, u64 n <= 9, u128 n <= 15,
 *            big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    /* Handle the offset explicitly */
    if (n == -1) return (seq_t)4;
    if (n == 0)  return (seq_t)5;
    if (n < -1)  return (seq_t)0;   /* undefined for indices below the offset */

    /* Product of the first n primes */
    seq_t prod = (seq_t)1;
    for (idx_t i = 1; i <= n; ++i) {
        /* get_prime returns the i‑th prime (1‑based); cast it to seq_t */
        seq_t p = (seq_t)get_prime((idx_t)i);
        prod = prod * p;
    }

    /* a(n) = 4 + (prod)² */
    seq_t val = prod * prod;
    val += (seq_t)4;
    return val;
}
