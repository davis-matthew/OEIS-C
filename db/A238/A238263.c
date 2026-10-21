/*
 * A238263  a(n) is the number of ways n can be written in the form
 *          n=2^k1*p1^k2+2^k3*p2^k4, where p1 and p2 are odd prime numbers,
 *          and k1, k2, k3, k4 are nonnegative integers.
 * https://oeis.org/A238263
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1994, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime_power_odd, is_representable_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * a(n) = number of unordered pairs (a,b) with a+b=n, a,b ≥ 1,
 * where each of a and b can be written as 2^k * p^j with p an odd prime,
 * k,j ≥ 0. This is computed by brute‑force scanning of the first ⌊n/2⌋ integers.
 */
#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_prime_power_odd.h"
#include "../utils/is_representable_uint64.h"

/* Main OEIS sequence function. */
seq_t a(idx_t n)
{
    if (n < 2) return 0;                     /* values below the offset are undefined */

    seq_t count = 0;
    for (idx_t a = 1; a <= n / 2; ++a) {
        idx_t b = n - a;
        if (is_representable_uint64((uint64_t)a) &&
            is_representable_uint64((uint64_t)b))
            ++count;
    }
    return count;
}
