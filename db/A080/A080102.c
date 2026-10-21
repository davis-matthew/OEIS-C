/*
 * A080102  Smallest prime power in all composite numbers between n-th prime
 *          and next prime, a(n) = 1 if no such prime power exists.
 * https://oeis.org/A080102
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = smallest composite prime power between the n-th prime and
 *            the next prime; 1 if no such prime power exists. (OEIS A080102)
 * Range:     u8 n <= 53, u16 n <= 6541, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

static void init_sieve(void);

/* Largest prime we will consider (enough for n up to about 10000). */
#define SIEVE_LIMIT 200000

static unsigned *spf;          /* smallest prime factor for each integer */
static unsigned *primes;       /* list of primes */
static size_t prime_count = 0;

static void init_sieve(void)
{
    if (spf != NULL) return;   /* already initialized */

    /* Allocate memory for sieve arrays */
    spf = (unsigned *)malloc((SIEVE_LIMIT + 1) * sizeof(unsigned));
    primes = (unsigned *)malloc((SIEVE_LIMIT / 10) * sizeof(unsigned));
    if (!spf || !primes) exit(1);   /* out of memory */
    memset(spf, 0, (SIEVE_LIMIT + 1) * sizeof(unsigned));

    /* Linear sieve to fill spf[] and primes[] */
    for (unsigned i = 2; i <= SIEVE_LIMIT; ++i) {
        if (spf[i] == 0) {
            spf[i] = i;
            primes[prime_count++] = i;
        }
        for (size_t j = 0; j < prime_count && primes[j] <= spf[i] && (unsigned)i * primes[j] <= SIEVE_LIMIT; ++j) {
            spf[i * primes[j]] = primes[j];
        }
    }
}

/* Return the least composite prime power between the n-th and (n+1)-st primes.
   n >= 2.  The result fits in seq_t because the primes involved are < 2*10^5
   (so the prime powers are < 2*10^10, well within 64‑bit). */
seq_t a(idx_t n)
{
    if (n < 2) return 1;          /* undefined for n < 2, return 1 */

    init_sieve();

    /* Need primes[n-1] and primes[n] (1‑based indices). */
    if ((size_t)n >= prime_count) return 1;   /* safety check */

    unsigned p = primes[n - 1];
    unsigned q = primes[n];

    unsigned min_prime_power = 0;

    for (unsigned m = p + 1; m < q; ++m) {
        unsigned r = spf[m];
        if (r == m) continue;     /* m is prime, skip */

        unsigned temp = m;
        while (temp % r == 0) temp /= r;
        if (temp == 1 && m > r) {   /* m = r^e, e >= 2 */
            if (min_prime_power == 0 || m < min_prime_power) {
                min_prime_power = m;
            }
        }
    }

    return (min_prime_power == 0) ? 1 : min_prime_power;
}
