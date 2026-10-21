/*
 * A078774  a(n) = n*phi(n*phi(n)).
 * https://oeis.org/A078774
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n*phi(n*phi(n))
 * Range:     u8 n <= 10, u16 n <= 45, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <string.h>

/* Prime sieve up to a fixed limit, used by the phi routine */
#define PRIME_SIEVE_LIMIT 200000
static uint64_t primes[PRIME_SIEVE_LIMIT];
static size_t primes_cnt = 0;
static bool primes_inited = false;

static void init_primes(void)
{
    if (primes_inited) return;
    bool mark[PRIME_SIEVE_LIMIT + 1];
    memset(mark, true, sizeof(mark));
    mark[0] = mark[1] = false;
    for (size_t i = 2; i * i <= PRIME_SIEVE_LIMIT; ++i) {
        if (mark[i]) {
            for (size_t j = i * i; j <= PRIME_SIEVE_LIMIT; j += i)
                mark[j] = false;
        }
    }
    for (size_t i = 2; i <= PRIME_SIEVE_LIMIT; ++i) {
        if (mark[i]) primes[primes_cnt++] = i;
    }
    primes_inited = true;
}

/* Compute φ(m) for 64‑bit m (m ≤ 2^64−1).  The result is returned as seq_t. */
static seq_t phi_uint64(uint64_t m)
{
    init_primes();
    uint64_t result = m;
    uint64_t temp = m;
    for (size_t i = 0; i < primes_cnt && primes[i] * primes[i] <= temp; ++i) {
        uint64_t p = primes[i];
        if (temp % p == 0) {
            result -= result / p;
            while (temp % p == 0) temp /= p;
        }
    }
    if (temp > 1) result -= result / temp;
    return (seq_t)result;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    uint64_t n64 = (uint64_t)n;
    uint64_t phi_n = (uint64_t)phi_uint64(n64);
    uint64_t prod = n64 * phi_n;          /* n*φ(n)  (≤ ~4e7 for n≤1e4) */
    uint64_t phi_prod = (uint64_t)phi_uint64(prod);
    return (seq_t)(n64 * phi_prod);       /* n*φ(n*φ(n)) */
}
