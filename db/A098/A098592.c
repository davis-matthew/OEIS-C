/*
 * A098592  Number of primes between n*30 and (n+1)*30.
 * https://oeis.org/A098592
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = number of primes x such that 30*n < x ≤ 30*(n+1)
 * Range:     u8 n <= 105, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

static unsigned char *is_prime = NULL;   /* 1 = prime, 0 = composite */
static idx_t *prefix = NULL;           /* prefix[i] = #primes ≤ i */
static idx_t max_limit = 0;            /* maximum index currently sieved */

/* Extend the sieve so that we have data up to new_limit (inclusive).  */
static void extend_sieve(idx_t new_limit)
{
    unsigned char *new_is_prime;
    idx_t *new_prefix;
    idx_t i, j;
    idx_t sqrt_limit = (idx_t)((double)new_limit * 0.5);

    /* Allocate new arrays */
    new_is_prime = (unsigned char *)malloc((size_t)(new_limit + 1));
    new_prefix = (idx_t *)malloc((size_t)(new_limit + 1) * sizeof(idx_t));

    /* Initialise all entries to prime */
    memset(new_is_prime, 1, (size_t)(new_limit + 1));

    /* 0 and 1 are not primes */
    if (new_limit >= 0) new_is_prime[0] = 0;
    if (new_limit >= 1) new_is_prime[1] = 0;

    /* Eratosthenes sieve */
    for (i = 2; i * i <= new_limit; ++i) {
        if (new_is_prime[i]) {
            for (j = i * i; j <= new_limit; j += i)
                new_is_prime[j] = 0;
        }
    }

    /* Build prefix counts */
    new_prefix[0] = 0;
    for (i = 1; i <= new_limit; ++i) {
        new_prefix[i] = new_prefix[i - 1] + (new_is_prime[i] ? 1 : 0);
    }

    /* Free old arrays if they exist */
    if (is_prime) free(is_prime);
    if (prefix)  free(prefix);

    /* Install new arrays */
    is_prime = new_is_prime;
    prefix   = new_prefix;
    max_limit = new_limit;
}

seq_t a(idx_t n)
{
    /* Ensure that sieve covers up to 30*(n+1) */
    idx_t high = (n + 1) * 30;
    idx_t low  = n * 30;

    if (high > max_limit) {
        extend_sieve(high);
    }

    return (seq_t)(prefix[high] - prefix[low]);
}
