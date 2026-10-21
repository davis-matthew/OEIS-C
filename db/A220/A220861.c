/*
 * A220861  Choose smallest m>0 such that the n-th rational prime p ramifies
 *          in the imaginary quadratic extension field K = Q(sqrt(-m)); a(n) =
 *          discriminant(K).
 * https://oeis.org/A220861
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = -4 if p=2; -p if p≡3 (mod 4); -4p if p≡1 (mod 4)
 * Range:     i8 n <= 11, i16 n <= 1000, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Uses:      get_prime(idx_t)  – returns the n‑th prime (p₁=2) */

#include "../utils/get_prime.h"
#include <stdint.h>

seq_t a(idx_t n)
{
    uint32_t p = get_prime(n);           /* nth rational prime */
    if (p == 2)                          /* only p=2 has m=1 */
        return (seq_t)-4;

    if ((p & 3) == 3)                    /* p ≡ 3 (mod 4)  */
        return -(seq_t)p;

    /* p ≡ 1 (mod 4) */
    int64_t val = -(int64_t)4 * (int64_t)p;
    return (seq_t)val;
}
