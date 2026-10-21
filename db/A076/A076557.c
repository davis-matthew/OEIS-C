/*
 * A076557  Greatest prime divisor of (n-th prime - n).
 * https://oeis.org/A076557
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) is the greatest prime divisor of (prime(n) – n)
 * Range:     u8 n <= 67, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* prime(n) is the n‑th prime (1‑based); offset is 3, so a(3) is the first
   listed term.  The implementation builds a linear sieve once and uses it
   to factor the difference. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

#define SIEVE_LIMIT 200000U          /* enough for n up to about 10000 */

static uint32_t *spf = NULL;         /* smallest prime factor table */
static uint32_t *primes = NULL;      /* array of primes (0‑based) */
static size_t       prime_cnt = 0;   /* number of primes found */

/* initialise the sieve only once */
static void init_sieve(void)
{
    if (spf != NULL) return;          /* already initialised */

    /* allocate smallest prime factor array */
    spf = (uint32_t*)calloc(SIEVE_LIMIT + 1, sizeof(uint32_t));
    /* allocate prime list – π(200000) < 17984 */
    primes = (uint32_t*)malloc((SIEVE_LIMIT / 10) * sizeof(uint32_t));

    size_t pc = 0;
    for (size_t i = 2; i <= SIEVE_LIMIT; ++i) {
        if (spf[i] == 0) {
            spf[i] = (uint32_t)i;
            primes[pc++] = (uint32_t)i;
        }
        for (size_t j = 0; j < pc && primes[j] <= spf[i] && i * primes[j] <= SIEVE_LIMIT; ++j) {
            spf[i * primes[j]] = primes[j];
        }
    }
    prime_cnt = pc;
}

/* return the largest prime factor of x (x >= 2).  x is guaranteed to be <= SIEVE_LIMIT. */
static uint32_t largest_prime_factor(uint64_t x)
{
    uint32_t largest = 1U;
    while (x > 1ULL) {
        uint32_t p = spf[(size_t)x];
        largest = p;
        while (x % p == 0ULL)
            x /= p;
    }
    return largest;
}

/* --------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n < 3) return (seq_t)0;   /* behaviour for n < 3 is unspecified */

    init_sieve();                 /* ensure primes are ready */

    /* nth prime (1‑based index) */
    uint32_t pn = primes[(size_t)n - 1U];

    /* difference d = prime(n) – n */
    uint64_t d = (uint64_t)pn - (uint64_t)n;

    /* greatest prime divisor of d */
    uint32_t gpf = largest_prime_factor(d);

    return (seq_t)gpf;
}
