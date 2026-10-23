/*
 * A126287  a(1) = 1, a(2) = 1, a(n) = n * LeastPrimeFactor(n-1) /
 *          LeastPrimeFactor(n).
 * https://oeis.org/A126287
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = n * LeastPrimeFactor(n‑1) / LeastPrimeFactor(n)
 * Range:     u8 n <= 23, u16 n <= 367, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* Return the least prime factor of n (n ≥ 2).
   For n ≤ 1 this function returns 1, which is harmless\n   because the caller handles n ≤ 2 specially.   */
static uint64_t least_prime_factor_uint64(uint64_t n)
{
    if (n <= 1) return 1;
    if ((n & 1) == 0) return 2;          /* even numbers */
    for (uint64_t d = 3; d * d <= n; d += 2) {
        if (n % d == 0) return d;
    }
    return n;                            /* n is prime */
}

seq_t a(idx_t n)
{
    if (n <= 2) return 1;                /* a(1) = a(2) = 1 */
    uint64_t spf_n   = least_prime_factor_uint64((uint64_t)n);
    uint64_t spf_np1 = least_prime_factor_uint64((uint64_t)(n - 1));
    return (seq_t)n * (seq_t)spf_np1 / (seq_t)spf_n;
}
