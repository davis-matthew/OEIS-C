/*
 * A068503  Highest power of 3 dividing prime(n)-1.
 * https://oeis.org/A068503
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 3^v where v is the exponent of 3 in prime(n)-1
 * Range:     u8 n <= 231, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>
#include <math.h>

static idx_t *prime_list = NULL;    /* 0‑based list of primes */
static size_t prime_cnt  = 0;        /* number of primes stored */
static size_t sieve_limit = 0;       /* current size of sieve */

/* Simple Eratosthenes sieve up to 'limit'.  After building, prime_list is
   resized to contain exactly the primes found, and prime_cnt is set. */
static void build_primes(idx_t n_needed)
{
    /* skip if already have enough primes */
    if (prime_cnt > (size_t)n_needed) return;

    /* estimate an upper bound for the n_needed‑th prime */
    size_t new_limit;
    if (n_needed < 6)
        new_limit = 15;
    else {
        double dn = (double)n_needed;
        new_limit = (size_t)(dn * (log(dn) + log(log(dn))) + 10.0);
    }
    if (new_limit <= sieve_limit) return; /* nothing to do */

    /* Allocate and run the sieve */
    unsigned char *sieve = (unsigned char *)malloc(new_limit + 1);
    memset(sieve, 0, new_limit + 1);
    size_t i, j;
    for (i = 2; i * i <= new_limit; ++i) {
        if (!sieve[i]) {
            for (j = i * i; j <= new_limit; j += i)
                sieve[j] = 1;
        }
    }

    /* Count primes to allocate storage */
    size_t new_cnt = 0;
    for (i = 2; i <= new_limit; ++i)
        if (!sieve[i]) ++new_cnt;

    /* Allocate/resize prime list */
    if (prime_list) free(prime_list);
    prime_list = (idx_t *)malloc(new_cnt * sizeof(idx_t));
    size_t idx = 0;
    for (i = 2; i <= new_limit; ++i) {
        if (!sieve[i]) {
            prime_list[idx++] = (idx_t)i;
        }
    }
    prime_cnt = new_cnt;
    sieve_limit = new_limit;

    free(sieve);
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* undefined, but keep harmless */
    build_primes(n);                       /* ensure primes up to index n */

    /* get the nth prime (offset 1) */
    idx_t p = prime_list[n - 1];

    /* compute exponent of 3 in p-1 */
    idx_t t = p - 1;
    int exp = 0;
    while (t % 3 == 0) {
        ++exp;
        t /= 3;
    }

    /* compute 3^exp */
    seq_t result = 1;
    while (exp-- > 0) result *= (seq_t)3;
    return result;
}
