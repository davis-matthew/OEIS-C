/*
 * A169606  a(2n-1) = prime(n+2)-3, a(2n) = prime(n+2)-2.
 * https://oeis.org/A169606
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(2n-1) = prime(n+2)-3, a(2n) = prime(n+2)-2
 * Range:     u8 n <= 106, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/get_prime.h"

seq_t a(idx_t n) {
    /* For n < 1 the sequence is undefined. Return 0 for safety. */
    if (n == 0) {
        return (seq_t)0;
    }

    /* Determine the index of the needed prime:
       For n = 2k-1 (odd)  -> prime(k+2)
       For n = 2k   (even) -> prime(k+2)
       The common formula is (n + 1) / 2 + 2. */
    idx_t prime_index = (n + 1) / 2 + 2;

    /* Retrieve the k‑th prime (k ≥ 3 here). */
    uint32_t p = get_prime(prime_index);
    seq_t prime = (seq_t)p;

    /* Subtract 3 for odd indices, 2 for even indices. */
    if (n & 1U) {           /* n is odd: a(2k-1) = prime(k+2) - 3 */
        prime -= (seq_t)3;
    } else {                /* n is even: a(2k)   = prime(k+2) - 2 */
        prime -= (seq_t)2;
    }
    return prime;
}
