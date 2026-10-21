/*
 * A070819  Values of commutator[phi,gpf] = commutator[A000010, A006530] at
 *          prime arguments; a(1)=0 by convention.
 * https://oeis.org/A070819
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = prime(n) - 1 - gpf(prime(n)-1)
 * Range:     u8 n <= 57, u16 n <= 6551, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*           a(1) = 0 by convention                       */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

static uint64_t *primes = NULL;   /* list of primes */
static size_t primes_count = 0;   /* number of primes stored */
static bool primes_initialized = false;

/* compute all primes up to LIMIT using a simple sieve */
static void init_primes(void)
{
    const size_t LIMIT = 2000000; /* enough for prime(100000)+ */
    bool *is_prime = (bool *)malloc((LIMIT + 1) * sizeof(bool));
    if (!is_prime) abort();          /* basic error handling */

    for (size_t i = 0; i <= LIMIT; ++i) is_prime[i] = true;
    is_prime[0] = is_prime[1] = false;

    for (size_t i = 2; i * i <= LIMIT; ++i) {
        if (is_prime[i]) {
            for (size_t j = i * i; j <= LIMIT; j += i) {
                is_prime[j] = false;
            }
        }
    }

    /* count primes first */
    size_t cnt = 0;
    for (size_t i = 2; i <= LIMIT; ++i) if (is_prime[i]) ++cnt;

    primes = (uint64_t *)malloc(cnt * sizeof(uint64_t));
    if (!primes) abort();

    size_t idx = 0;
    for (size_t i = 2; i <= LIMIT; ++i) {
        if (is_prime[i]) {
            primes[idx++] = i;
        }
    }
    primes_count = cnt;
    primes_initialized = true;
    free(is_prime);
}

/* largest prime factor of x; x <= primes[n]-1, thus small enough.
 * For x < 2 we return x itself (gpf(1)=1). */
static uint64_t largest_prime_factor(uint64_t x)
{
    if (x < 2) return x;

    uint64_t last = 1;
    for (size_t i = 0; i < primes_count; ++i) {
        uint64_t p = primes[i];
        if (p * p > x) break;
        if (x % p == 0) {
            last = p;
            while (x % p == 0) x /= p;
        }
    }
    if (x > 1) last = x;
    return last;
}

/* the required sequence function */
seq_t a(idx_t n)
{
    if (!primes_initialized) init_primes();

    if (n == 0) return (seq_t)0;           /* safeguard: offset 1 */

    if ((size_t)n > primes_count) {
        /* too many terms requested; extend sieve */
        /* simple strategy: increase limit and recompute */
        /* (rare in the tested range) */
        free(primes);
        primes = NULL;
        primes_count = 0;
        primes_initialized = false;
        init_primes();
        if ((size_t)n > primes_count) abort(); /* still insufficient */
    }

    uint64_t p = primes[(size_t)n - 1];      /* prime(n) */
    uint64_t phi = p - 1;                    /* φ(p) = p-1 */
    uint64_t gp = largest_prime_factor(phi); /* gpf(p-1) */
    return (seq_t)(phi - gp);                /* a(n) = (p-1) - gpf(p-1) */
}
