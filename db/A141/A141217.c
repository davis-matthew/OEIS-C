/*
 * A141217  a(n) = prime(prime(prime(prime(n) - 1) - 1) - 1) - 1, where
 *          prime(n) is the n-th prime.
 * https://oeis.org/A141217
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 34, u32 n <= 38, u64 n <= 38, u128 n <= 38,
 *            big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = prime(prime(prime(prime(n) - 1) - 1) - 1) - 1
 * In OEIS notation, prime(k) denotes the k‑th prime (1‑based indexing).
 */

#include <stdint.h>
#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;   /* undefined for n < 1, not used by OEIS */

    /* Compute each nested prime step, keeping intermediate values in uint64_t
       to avoid overflow for the bounds required by this sequence. */
    uint64_t x1 = (uint64_t)get_prime(n);
    uint64_t x2 = (uint64_t)get_prime((idx_t)(x1 - 1));
    uint64_t x3 = (uint64_t)get_prime((idx_t)(x2 - 1));
    uint64_t x4 = (uint64_t)get_prime((idx_t)(x3 - 1));

    return (seq_t)(x4 - 1);
}
