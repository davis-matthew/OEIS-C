/*
 * A122416  Numbers from an irrationality measure for e, with a(1) = 2.
 * https://oeis.org/A122416
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = S(n) + 1, where S(n) is the smallest k such that n | k!
 * Range:     u8 n <= 81, u16 n <= 81, u32 n <= 81, u64 n <= 81, u128 n <= 81,
 *            big any n
 * Helpers:   minimal_k_for_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/minimal_k_for_prime.h"

seq_t a(idx_t n)
{
    /* For n = 1, the smallest k with 1 | k! is 1, thus a(1)=2. */
    if (n == 0)  /* safety for out‑of‑range calls */
        return (seq_t)2;

    idx_t remaining = n;
    idx_t max_k = 1;             /* minimal k will be at least 1 */

    for (idx_t p = 2; p <= remaining / p; ++p) {
        if (remaining % p == 0) {
            idx_t exp = 0;
            while (remaining % p == 0) {
                remaining /= p;
                ++exp;
            }
            idx_t k = minimal_k_for_prime(p, exp);
            if (k > max_k)
                max_k = k;
        }
    }

    /* If remaining > 1, it is a prime factor with exponent 1. */
    if (remaining > 1) {
        idx_t k = minimal_k_for_prime(remaining, 1);
        if (k > max_k)
            max_k = k;
    }

    return (seq_t)(max_k + 1);
}
