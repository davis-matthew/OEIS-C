/*
 * A099618  a(n) = 1 if the n-th prime == 1 mod 6, otherwise a(n) = 0.
 * https://oeis.org/A099618
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1 if the n‑th prime ≡ 1 (mod 6), otherwise 0
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

/* ----------------------------------- helper ----------------------------------- */
/* Rough upper bound for the n‑th prime (enough for n up to many millions)      */
static size_t estimate_limit(size_t n)
{
    if (n < 6) return 15;
    /* For n ≥ 6, n*(ln n + ln ln n) < 25 n for n < 2 000 000, so 25 n is safe  */
    return n * 25 + 3;
}

/* Global prime cache ---------------------------------------------------------- */
static uint32_t *prime_cache   = NULL;   /* array of primes */
static size_t   prime_count   = 0;       /* number of stored primes   */
static size_t   prime_capacity = 0;     /* allocated capacity        */

/* Rebuild the prime cache to contain at least 'need' primes.                */
static void build_primes(size_t need)
{
    size_t limit = estimate_limit(need);

    /* sieve of Eratosthenes on an 8‑bit array (0 = prime, 1 = composite)   */
    uint8_t *sieve = (uint8_t *)calloc(limit + 1, 1);
    if (!sieve) abort();   /* unrecoverable */

    /* temporary prime storage – grows as needed */
    size_t cap = 8;
    uint32_t *tmp = (uint32_t *)malloc(cap * sizeof(uint32_t));
    if (!tmp) { free(sieve); abort(); }

    size_t cnt = 0;
    for (size_t p = 2; p <= limit; ++p) {
        if (!sieve[p]) {
            if (cnt == cap) {            /* enlarge temporary storage */
                cap <<= 1;
                tmp = (uint32_t *)realloc(tmp, cap * sizeof(uint32_t));
                if (!tmp) { free(sieve); abort(); }
            }
            tmp[cnt++] = (uint32_t)p;

            if (p * p <= limit) {
                for (size_t mult = p * p; mult <= limit; mult += p)
                    sieve[mult] = 1;
            }
        }
    }

    free(sieve);

    /* replace old cache with the new one */
    free(prime_cache);
    prime_cache   = tmp;
    prime_count   = cnt;
    prime_capacity = cap;
}

/* ----------------------------------- main ----------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* out of bounds, harmless */

    /* ensure we have at least 'n' primes cached */
    if (prime_count < (size_t)n) {
        build_primes((size_t)n);
    }

    uint32_t p = prime_cache[(size_t)n - 1];   /* nth prime, 1‑based */
    return ((p % 6U) == 1U) ? (seq_t)1 : (seq_t)0;
}
