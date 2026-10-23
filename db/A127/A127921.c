/*
 * A127921  1/12 of product of three numbers: n-th prime, previous and
 *          following number.
 * https://oeis.org/A127921
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = p_n*(p_n-1)*(p_n+1)/12 where p_n is the n-th prime
 * Range:     u8 n <= 3, u16 n <= 12, u32 n <= 277, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

#define PRIME_LIMIT 200000        /* enough for the 10 000‑th prime */
#define MAX_PRIMES 17000          /* number of primes below PRIME_LIMIT */

/* Prime table and sieve data */
static uint64_t primes[MAX_PRIMES];
static size_t primes_cnt = 0;
static bool primes_inited = false;

/* One‑time sieve to fill the prime table */
static void init_primes(void)
{
    if (primes_inited) return;

    static uint8_t is_prime[PRIME_LIMIT + 1];
    memset(is_prime, 1, PRIME_LIMIT + 1);
    is_prime[0] = is_prime[1] = 0;

    /* Eratosthenes */
    for (uint64_t i = 2; i * i <= PRIME_LIMIT; ++i) {
        if (is_prime[i]) {
            for (uint64_t j = i * i; j <= PRIME_LIMIT; j += i)
                is_prime[j] = 0;
        }
    }

    /* Store primes in order */
    size_t idx = 0;
    for (uint64_t i = 2; i <= PRIME_LIMIT && idx < MAX_PRIMES; ++i)
        if (is_prime[i]) {
            primes[idx++] = i;
        }

    primes_cnt = idx;
    primes_inited = true;
}

/* OEIS A127921: 1/12 of product of three numbers: the n‑th prime, the
 * previous integer, and the following integer. */
seq_t a(idx_t n)
{
    if (n < 2) return 0;          /* offset starts at 2 (behaviour below is irrelevant) */

    init_primes();

    if ((size_t)(n - 1) >= primes_cnt)
        return 0;                 /* n beyond precomputed range – not expected for n ≤ 10000 */

    uint64_t p = primes[n - 1];
    seq_t p_seq = (seq_t)p;

    seq_t prod = p_seq * (p_seq - 1) * (p_seq + 1); /* fits in seq_t for n ≤ 10000 */
    return prod / 12;
}
