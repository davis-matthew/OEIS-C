/*
 * A033475  Denominator of Bernoulli(2n,1/4).
 * https://oeis.org/A033475
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2^(4n) * Π { p odd prime : (p‑1) | 2n }
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 13, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

/* precompute primes up to 20001 once */
static bool primes_initialized = false;
static uint32_t primes[20002];
static size_t primes_count = 0;

static void init_primes(void)
{
    bool is_prime[20002];
    for (int i = 0; i <= 20001; i++) is_prime[i] = true;
    is_prime[0] = is_prime[1] = false;
    for (int i = 2; i * i <= 20001; i++) {
        if (is_prime[i]) {
            for (int j = i * i; j <= 20001; j += i) is_prime[j] = false;
        }
    }
    for (int i = 2; i <= 20001; i++)
        if (is_prime[i]) primes[primes_count++] = (uint32_t)i;
    primes_initialized = true;
}

seq_t a(idx_t n)
{
    if (n == 0) return 1;

    if (!primes_initialized) init_primes();

    /* compute 2^(4n) = 16^n */
    seq_t res = 1;
    for (idx_t i = 0; i < n; ++i) res *= 16;

    idx_t m = 2 * n;

    for (size_t i = 0; i < primes_count; ++i) {
        uint32_t p = primes[i];
        if (p <= 2) continue;              /* only odd primes */
        if (m % (p - 1) == 0) {
            res *= (seq_t)p;
        }
    }
    return res;
}
