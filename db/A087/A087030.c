/*
 * A087030  n "reflected" in the next prime: a(n)=2p-n, p is smallest prime >
 *          n.
 * https://oeis.org/A087030
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 2*p - n where p is the smallest prime > n
 * Range:     u8 n <= 240, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* Simple deterministic primality test for 64‑bit numbers (sufficient for n ≤ 10000). */
static bool is_prime_uint64(uint64_t n)
{
    if (n < 2)               return false;
    if (n % 2 == 0)          return n == 2;
    for (uint64_t d = 3; d <= n / d; d += 2)
        if (n % d == 0)
            return false;
    return true;
}

seq_t a(idx_t n)
{
    idx_t p = n + 1;
    while (!is_prime_uint64(p))
        ++p;

    return ((seq_t)2 * (seq_t)p) - (seq_t)n;
}
