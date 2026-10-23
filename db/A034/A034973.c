/*
 * A034973  Number of distinct prime factors in central binomial coefficients
 *          C(n, floor(n/2)), the terms of A001405.
 * https://oeis.org/A034973
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   not stated
 * Range:     u8 n <= 2566, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute the number of distinct prime factors of the central binomial
   coefficient C(n, floor(n/2)). The result is omega(C(n, floor(n/2))).
   The function is memoised with a sieve that grows as needed. */
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

/* static storage for the prime sieve */
static uint64_t *prime_list = NULL;   /* array of primes */
static size_t   prime_count = 0;      /* number of primes stored */
static idx_t    sieve_limit = 0;      /* largest value up to which the sieve has been computed */

/* Build all primes ≤ limit with a classic sieve.  The array is
   resized only if the new limit exceeds the old one. */
static void sieve_primes_up_to(idx_t limit)
{
    if (limit <= sieve_limit)
        return;                          /* already sufficient */

    /* free any previous list */
    free(prime_list);
    prime_list = NULL;
    prime_count = 0;

    /* composite flag array */
    size_t sz = (size_t)(limit + 1);
    uint8_t *composite = (uint8_t *)malloc(sz);
    if (!composite)
        abort();
    memset(composite, 0, sz);

    idx_t root = (idx_t)sqrt((double)limit);
    for (idx_t i = 2; i <= root; ++i) {
        if (!composite[i]) {
            for (idx_t j = i * i; j <= limit; j += i)
                composite[j] = 1;
        }
    }

    /* count primes to allocate the array */
    size_t cnt = 0;
    for (idx_t i = 2; i <= limit; ++i)
        if (!composite[i])
            ++cnt;

    prime_list = (uint64_t *)malloc(cnt * sizeof(uint64_t));
    if (!prime_list)
        abort();

    size_t pos = 0;
    for (idx_t i = 2; i <= limit; ++i)
        if (!composite[i])
            prime_list[pos++] = (uint64_t)i;

    prime_count = cnt;
    sieve_limit = limit;
    free(composite);
}

/* Return the number of distinct prime factors of the central
   binomial coefficient C(n, floor(n/2)). */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;                 /* not used for offset 1 */

    idx_t k = n / 2;                      /* floor(n/2) */

    sieve_primes_up_to(n);                /* ensure primes up to n */

    int omega = 0;                        /* distinct prime factors counter */

    for (size_t pi = 0; pi < prime_count && prime_list[pi] <= n; ++pi) {
        uint64_t p = prime_list[pi];
        idx_t exp = 0;

        /* exponent of p in n! */
        idx_t m = n;
        while (m) {
            m /= p;
            exp += m;
        }

        /* subtract exponent of p in k! */
        idx_t mk = k;
        while (mk) {
            mk /= p;
            exp -= mk;
        }

        /* subtract exponent of p in (n-k)! */
        idx_t mnk = n - k;
        while (mnk) {
            mnk /= p;
            exp -= mnk;
        }

        if (exp > 0)
            ++omega;
    }

    return (seq_t)omega;
}
