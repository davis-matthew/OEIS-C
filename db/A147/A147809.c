/*
 * A147809  Half the number of proper divisors (> 1) of n^2 + 1, i.e., tau(n^2
 *          + 1)/2 - 1.
 * https://oeis.org/A147809
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = tau(n^2 + 1)/2 - 1 (tau = number of divisors)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* Static cache of primes up to 10 000.  This is more than enough for all
   n ≤ 10 000:  max(n^2+1) ≈ 1 000 001  → sqrt ≈ 1 000.  */
static uint32_t primes[1229];
static size_t   primes_cnt = 0;
static bool     primes_ready = false;

/* Build the prime table once. */
static void
init_primes(void)
{
    if (primes_ready)  return;

    static uint8_t sieve[10001] = {0};
    primes_cnt = 0;

    for (uint32_t i = 2; i <= 10000; ++i) {
        if (!sieve[i]) {
            primes[primes_cnt++] = i;
            if ((uint64_t)i * i <= 10000) {
                for (uint32_t j = i * i; j <= 10000; j += i)
                    sieve[j] = 1;
            }
        }
    }
    primes_ready = true;
}

/* Integer square root for uint64_t values. */
static uint64_t
int_sqrt_uint64(uint64_t n)
{
    uint64_t r = 0;
    while ((r + 1) * (r + 1) <= n) ++r;
    return r;
}

/* Main sequence function. */
seq_t
a(idx_t n)
{
    init_primes();                 /* ensure primes table exists   */
    if (n == 0) return 0;          /* defined only for n ≥ 1       */

    /* n^2 + 1 – fits comfortably in uint64_t for n ≤ 10 000. */
    uint64_t val = (uint64_t)n * (uint64_t)n + 1ull;
    uint64_t x   = val;
    seq_t      tau = 1;            /* divisor count                 */

    /* Trial division using the prime table. */
    for (size_t i = 0; i < primes_cnt && (uint64_t)primes[i] * primes[i] <= x; ++i) {
        uint32_t p  = primes[i];
        uint8_t  exp = 0;
        while (x % p == 0) {
            x /= p;
            ++exp;
        }
        if (exp > 0)
            tau = tau * (seq_t)(exp + 1);
    }

    /* If a remainder > 1, it is a prime factor. */
    if (x > 1)
        tau = tau * (seq_t)2;

    /* a(n) = (tau / 2) - 1  – tau is always even. */
    return tau / 2 - 1;
}
