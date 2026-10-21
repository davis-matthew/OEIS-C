/*
 * A075794  a(n) = the least positive integer k such that phi(k) > phi(1) +
 *          ... + phi(n).
 * https://oeis.org/A075794
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the least k such that φ(k) > φ(1)+...+φ(n)
 * Range:     u8 n <= 28, u16 n <= 463, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation precomputes a sieve of primes up to a fixed
   upper bound and uses a straightforward totient calculation
   by trial division with that prime list.  The function works for
   any seq_t and idx_t supplied by the build environment and
   is fully compliant with C11 and C++17. */
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

/* ----------  Prime sieve (once) ---------- */
#define PRIME_SIEVE_LIMIT 1000000U  /* big enough for all needed n */
static uint32_t *prime_list   = NULL;   /* array of primes */
static size_t    prime_count  = 0;      /* number of primes */

/* Initialise the prime list on first use */
static void init_primes(void)
{
    if (prime_list != NULL) return;          /* already initialised */

    const size_t limit = PRIME_SIEVE_LIMIT;
    bool *is_composite = (bool *)malloc(limit + 1);
    if (!is_composite) exit(1);
    memset(is_composite, 0, limit + 1);

    /* Count primes */
    size_t count = 0;
    for (size_t i = 2; i <= limit; ++i)
        if (!is_composite[i]) ++count;

    prime_list = (uint32_t *)malloc(count * sizeof(uint32_t));
    if (!prime_list) exit(1);

    /* Store primes */
    size_t idx = 0;
    for (size_t i = 2; i <= limit; ++i)
        if (!is_composite[i]) prime_list[idx++] = (uint32_t)i;

    /* Mark multiples */
    for (size_t i = 2; i * i <= limit; ++i)
        if (!is_composite[i]) {
            for (size_t j = i * i; j <= limit; j += i)
                is_composite[j] = true;
        }

    prime_count = count;
    free(is_composite);
}

/* ----------  Totient of a single integer ---------- */
static seq_t phi_single(idx_t n)
{
    if (n <= 1) return (seq_t)n;          /* φ(0)=0, φ(1)=1 */

    uint64_t result = (uint64_t)n;
    uint64_t m = (uint64_t)n;
    init_primes();

    for (size_t i = 0; i < prime_count && (uint64_t)prime_list[i] * prime_list[i] <= m; ++i) {
        uint32_t p = prime_list[i];
        if (m % p == 0) {
            while (m % p == 0) m /= p;
            result -= result / p;
        }
    }
    if (m > 1) {         /* remaining prime factor */
        result -= result / m;
    }
    return (seq_t)result;
}

/* ----------  Main sequence function ---------- */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;  /* defined only for n≥1, but guard against 0 */

    /* Compute S(n) = Σ_{i=1}^n φ(i) */
    uint64_t sum = 0;
    for (idx_t i = 1; i <= n; ++i) {
        sum += (uint64_t)phi_single(i);
    }

    /* Find the smallest k > sum with φ(k) > sum */
    uint64_t k = sum + 1;
    while (1) {
        if ((uint64_t)phi_single(k) > sum)
            return (seq_t)k;
        ++k;
    }
}
