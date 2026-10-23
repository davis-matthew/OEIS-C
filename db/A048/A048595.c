/*
 * A048595  Alternative start to A002371, which is the main entry for this
 *          sequence.
 * https://oeis.org/A048595
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, nice, base
 * Formula:   a(n) = length of the repeating part of the decimal expansion of
 *            the rational number n / p_n, where p_n is the n‑th prime. For
 *            p_n = 2 or 5 the period is defined to be 1; otherwise it equals
 *            the multiplicative order of 10 modulo p_n
 * Range:     u8 n <= 54, u16 n <= 67, u32 n <= 67, u64 n <= 67, u128 n <= 67,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#define MAX_PRIME_COUNT 20000   /* sufficient for n <= 10000 */
static uint32_t primes_arr[MAX_PRIME_COUNT];
static size_t   prime_count = 0;

/* ------------------------------------------------------------------
 * Simple prime sieve – generates at least |need| primes.
 * The implementation is deterministic and uses only standard C
 * operations; it is called at most once for the first invocation
 * of a().
 * ------------------------------------------------------------------ */
static void init_primes(size_t need)
{
    if (prime_count >= need)
        return;

    /* 200000 is an upper bound for the 10000th prime (104729) */
    const size_t limit = 200000;
    bool *sieve = (bool*)malloc(limit + 1);
    if (!sieve)
        return; /* allocation failure – unlikely in the test harness */

    memset(sieve, 0, limit + 1);
    for (size_t i = 2; i * i <= limit; ++i)
        if (!sieve[i])
            for (size_t j = i * i; j <= limit; j += i)
                sieve[j] = true;

    size_t count = 0;
    for (size_t i = 2; count < MAX_PRIME_COUNT && i <= limit; ++i)
        if (!sieve[i])
            primes_arr[count++] = (uint32_t)i;

    prime_count = count;
    free(sieve);
}

/* ------------------------------------------------------------------
 * Modular exponentiation for arbitrary integer type seq_t.
 * Uses only the operators +, -, *, /, %, <<, >> and cast to native
 * types when the size is guaranteed to be small.
 * ------------------------------------------------------------------ */
static inline seq_t mod_pow_seq(seq_t base, uint64_t exp, seq_t mod)
{
    seq_t result = 1;
    seq_t b = base % mod;
    while (exp) {
        if (exp & 1U)
            result = (result * b) % mod;
        b = (b * b) % mod;
        exp >>= 1U;
    }
    return result;
}

/* ------------------------------------------------------------------
 * Main sequence function
 * ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n < 1)
        return 0;                     /* offset 1; undefined for n<1 */

    init_primes((size_t)n);

    seq_t p = (seq_t)primes_arr[(size_t)n - 1];   /* p_n */

    if (p == 2 || p == 5)            /* terminating decimal */
        return (seq_t)1;

    /* Compute multiplicative order of 10 modulo p */
    uint32_t m = (uint32_t)p - 1;     /* p is prime, p>5 */
    uint64_t ord = m;
    uint32_t temp = m;

    /* Factor m using the precomputed prime list */
    for (size_t i = 0;
         i < prime_count && (uint64_t)primes_arr[i] * primes_arr[i] <= temp;
         ++i) {
        uint32_t q = primes_arr[i];
        if (temp % q == 0) {
            while (temp % q == 0)
                temp /= q;
            while (ord % q == 0 &&
                   mod_pow_seq((seq_t)10, ord / q, p) == (seq_t)1)
                ord /= q;
        }
    }

    /* Remaining prime factor > sqrt(m) */
    if (temp > 1) {
        uint32_t q = temp;
        while (ord % q == 0 &&
               mod_pow_seq((seq_t)10, ord / q, p) == (seq_t)1)
            ord /= q;
    }

    return (seq_t)ord;
}
