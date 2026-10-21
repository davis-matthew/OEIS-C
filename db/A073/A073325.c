/*
 * A073325  a(n) = least k > 0 such that prime(k) == n (mod k).
 * https://oeis.org/A073325
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = least k > 0 such that prime(k) == n (mod k)
 * Range:     u8 n <= 3, u16 n <= 17, u32 n <= 353, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    idx_t k = 1U;
    for (;; ++k) {
        uint32_t p = get_prime(k);          /* k‑th prime (1‑based). */
        seq_t r = (seq_t)p % (seq_t)k;      /* remainder of p modulo k. */
        if (r == (seq_t)n)                  /* satisfies the condition. */
            return (seq_t)k;                /* cast to result type. */
    }
}
