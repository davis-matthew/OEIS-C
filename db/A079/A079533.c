/*
 * A079533  Floor(k - sqrt(k)) - phi(k) as k runs through the composite
 *          numbers (A002808).
 * https://oeis.org/A079533
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look
 * Formula:   a(n) = floor(k - sqrt(k)) - φ(k) where k is the n‑th composite
 *            number
 * Range:     u8 n <= 44, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   floor_sqrt, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_prime.h"   /* bool is_prime(idx_t x); */
#include "../utils/floor_sqrt.h" /* seq_t floor_sqrt(seq_t x); */

/* Compute Euler's totient for a 64‑bit integer n.  The result fits in seq_t. */
static seq_t phi_uint64(uint64_t n)
{
    seq_t result = (seq_t)n;
    uint64_t t = n;
    for (uint64_t p = 2; p * p <= t; ++p)
    {
        if (t % p == 0)
        {
            while (t % p == 0) t /= p;
            result -= result / (seq_t)p;
        }
    }
    if (t > 1) result -= result / (seq_t)t;
    return result;
}

seq_t a(idx_t n)
{
    idx_t count = 0;
    idx_t k = 4;   /* first composite number */

    for (;; ++k)
    {
        if (!is_prime(k))
        {
            ++count;
            if (count == n)
            {
                seq_t K = (seq_t)k;
                seq_t s = floor_sqrt(K);
                seq_t sq = s * s;
                seq_t ceil_sqrt = (sq == K) ? s : s + 1;
                seq_t floor_val   = K - ceil_sqrt;
                seq_t phi_val     = phi_uint64((uint64_t)k);
                return floor_val - phi_val;
            }
        }
    }
}
