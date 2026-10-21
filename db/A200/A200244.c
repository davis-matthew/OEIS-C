/*
 * A200244  a(n)=1 iff binary weight of n-th prime is even.
 * https://oeis.org/A200244
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) equals 1 iff the binary weight of the n‑th prime is even
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute a(n) for n ≥ 1.  Uses the helper function
   uint32_t get_prime(idx_t k) from utils/get_prime.h. */
#include <stdbool.h>
#include <stdint.h>
#include "../utils/get_prime.h"

seq_t a(idx_t n) {
    /* Get the n‑th prime; the sequence is 1‑indexed. */
    uint32_t p = get_prime(n);

    /* Parity of the popcount: 0 = even, 1 = odd. */
    unsigned int parity = 0;
    while (p) {
        parity ^= 1;
        p &= p - 1;   /* clear the lowest set bit */
    }

    /* Return 1 when parity is even, otherwise 0. */
    return (seq_t)(parity ^ 1);
}
