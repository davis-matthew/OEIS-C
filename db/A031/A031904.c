/*
 * A031904  a(n) = prime(9*n - 4).
 * https://oeis.org/A031904
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = prime(9*n - 4)
 * Range:     u8 n <= 6, u16 n <= 43, u32 n <= 43, u64 n <= 43, u128 n <= 43,
 *            big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    /* n is 1‑based; for n < 1 the sequence is undefined, but we guard
       against underflow so that n == 0 yields 0 (behaviour below the
       offset is irrelevant). */
    if (n == 0)
        return 0;

    /* compute the required index of the prime */
    idx_t k = 9 * n - 4;          /* 9*n fits in 64‑bit for n up to 10000 */

    /* retrieve the k‑th prime (k ≥ 1) – the utility guarantees a
       uint32_t return value, which comfortably covers the required range
       (the 90 000‑th prime is ~1.25 × 10⁶). */
    uint32_t p = get_prime(k);

    /* return as seq_t; cast is safe for any seq_t width up to 128 bits */
    return (seq_t)p;
}
