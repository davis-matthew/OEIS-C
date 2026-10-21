/*
 * A076490  Number of common (distinct) digits of consecutive prime numbers.
 * https://oeis.org/A076490
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = number of common distinct digits of the n‑th and (n+1)-th
 *            primes (OEIS A076490)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

/* Prime sieve: we generate primes up to a fixed bound that covers
   at least the first 10001 primes (the 10001st prime is 104743). */
#define PRIME_LIMIT 200000
static uint64_t *g_primes = NULL;
static size_t   g_prime_count = 0;
static bool     g_primes_ready = false;

/* Build a list of primes up to PRIME_LIMIT on first use. */
static void init_primes(void)
{
    if (g_primes_ready)
        return;
    g_primes_ready = true;

    /* Allocate and initialise the sieve array to "prime". */
    char *is_prime = (char *)malloc(PRIME_LIMIT + 1);
    if (!is_prime)
        return;                 /* memory error – abort */
    for (size_t i = 0; i <= PRIME_LIMIT; ++i)
        is_prime[i] = 1;         /* assume prime */
    is_prime[0] = is_prime[1] = 0;   /* 0 and 1 are not prime */

    /* Eratosthenes sieve */
    for (size_t i = 2; i * i <= PRIME_LIMIT; ++i)
        if (is_prime[i])
            for (size_t j = i * i; j <= PRIME_LIMIT; j += i)
                is_prime[j] = 0;

    /* Count primes to allocate the array. */
    size_t temp_cnt = 0;
    for (size_t i = 2; i <= PRIME_LIMIT; ++i)
        if (is_prime[i])
            ++temp_cnt;

    /* Allocate array of primes. */
    g_primes = (uint64_t *)malloc(temp_cnt * sizeof(uint64_t));
    if (!g_primes) {
        free(is_prime);
        g_prime_count = 0;
        return;
    }
    g_prime_count = temp_cnt;
    size_t idx = 0;
    for (size_t i = 2; i <= PRIME_LIMIT; ++i)
        if (is_prime[i])
            g_primes[idx++] = i;

    free(is_prime);
}

/* Compute a bitmask of the digits present in an integer. */
static inline uint16_t digit_mask(uint64_t x)
{
    uint16_t m = 0;
    if (x == 0)
        return 1u;           /* digit 0 */
    while (x) {
        m |= 1u << (x % 10);
        x /= 10;
    }
    return m;
}

/* seq_t a(idx_t n) implementation. */
seq_t a(idx_t n)
{
    /* Guard against out‑of‑range requests. */
    if (n < 1)
        return (seq_t)0;

    init_primes();

    /* We need the (n)-th and (n+1)-th primes. */
    if (g_prime_count < n + 1)
        return (seq_t)0;         /* not enough primes generated */

    uint64_t p1 = g_primes[n - 1];
    uint64_t p2 = g_primes[n];

    uint16_t m1 = digit_mask(p1);
    uint16_t m2 = digit_mask(p2);
    uint16_t common = m1 & m2;

    seq_t cnt = 0;
    while (common) {
        ++cnt;
        common &= common - 1;   /* clear lowest set bit */
    }
    return cnt;
}
