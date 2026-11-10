/*
 * A091276  A090939(n)/n.
 * https://oeis.org/A091276
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ((prime(n)-1) * n^(-1) mod prime(n)), i.e. A090939(n)/n:
 *            the least k >= 1 with k*n == -1 (mod prime(n))
 * Range:     u8 n <= 62, u16 n <= 67, u32 n <= 67, u64 n <= 67, u128 n <= 67,
 *            big any n
 * Validated: partial, pass, 67 terms through n = 67 (2026-11-07)
 * Helpers:   nth_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/nth_prime.h"

static uint64_t A091276_inv(uint64_t x, uint64_t m)
{
    long long a0 = (long long)(x % m), b0 = (long long)m;
    long long x0 = 1, x1 = 0;
    while (b0 != 0) {
        long long q = a0 / b0;
        long long t = a0 - q * b0; a0 = b0; b0 = t;
        t = x0 - q * x1; x0 = x1; x1 = t;
    }
    x0 %= (long long)m;
    if (x0 < 0) x0 += (long long)m;
    return (uint64_t)x0;
}

seq_t a(idx_t n)
{
    uint64_t p = nth_prime(n);
    uint64_t inv = A091276_inv((uint64_t)(n % p), p);
    uint64_t k = (uint64_t)(((unsigned __int128)(p - 1) * inv) % p);
    if (k == 0) k = p;
    return (seq_t)k;
}
