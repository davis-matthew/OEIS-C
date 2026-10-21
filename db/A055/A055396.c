/*
 * A055396  Smallest prime dividing n is a(n)-th prime (a(1)=0).
 * https://oeis.org/A055396
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = index of the smallest prime factor of n (a(1)=0). The
 *            index counts primes starting at 2 as the first prime
 * Range:     u8 n <= 1618, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

/* ------------------------------------------------------------------ */
/*   Prime cache                                                     */
/* ------------------------------------------------------------------ */

static size_t prime_limit = 0;           /* largest number for which primes are computed */
static size_t prime_count = 0;           /* number of primes stored */
static idx_t *prime_list = NULL;         /* 1‑based indices: prime_list[0] == 2 */

/* Ensure that prime_list contains all primes <= limit.  The list is
   rebuilt if a larger limit is requested. */
static void ensure_primes_up_to(idx_t limit)
{
    if (limit <= prime_limit)
        return;                         /* already sufficient */

    /* Allocate sieve array */
    size_t sieve_size = (size_t)limit + 1;
    unsigned char *sieve = (unsigned char *)malloc(sieve_size);
    if (!sieve)
        return; /* allocation failure – unlikely for limits <= 10000 */

    /* Simple sieve of Eratosthenes */
    for (size_t i = 0; i < sieve_size; ++i)
        sieve[i] = 1;
    sieve[0] = sieve[1] = 0;

    for (size_t i = 2; i * i <= (size_t)limit; ++i) {
        if (sieve[i]) {
            for (size_t j = i * i; j <= (size_t)limit; j += i)
                sieve[j] = 0;
        }
    }

    /* Count primes */
    size_t cnt = 0;
    for (size_t i = 2; i <= (size_t)limit; ++i)
        if (sieve[i])
            ++cnt;

    /* Allocate new prime list */
    idx_t *new_list = (idx_t *)malloc(cnt * sizeof(idx_t));
    if (!new_list) {
        free(sieve);
        return; /* allocation failure */
    }

    /* Fill list */
    size_t idx = 0;
    for (size_t i = 2; i <= (size_t)limit; ++i) {
        if (sieve[i])
            new_list[idx++] = (idx_t)i;
    }

    /* Replace old list */
    free(prime_list);
    prime_list = new_list;
    prime_count = cnt;
    prime_limit = limit;

    free(sieve);
}

/* Binary search for prime p in prime_list.  Returns 1‑based index or 0 if
   not found (should never happen for a prime factor). */
static seq_t index_of_prime(idx_t p)
{
    size_t lo = 0, hi = prime_count;
    while (lo < hi) {
        size_t mid = (lo + hi) >> 1;
        idx_t val = prime_list[mid];
        if (val < p)
            lo = mid + 1;
        else if (val > p)
            hi = mid;
        else
            return (seq_t)(mid + 1);  /* indices are 1‑based */
    }
    return (seq_t)0;
}

/* ------------------------------------------------------------------ */
/*   A055396 implementation                                         */
/* ------------------------------------------------------------------ */

seq_t a(idx_t n)
{
    if (n <= 1)
        return (seq_t)0;                 /* a(1) = 0 by definition */

    /* Make sure we have all primes up to n for the index lookup. */
    ensure_primes_up_to(n);

    /* Find smallest prime factor by trial division over primes. */
    idx_t spf = n;   /* default: n itself (prime case) */
    for (size_t i = 0; i < prime_count; ++i) {
        idx_t p = prime_list[i];
        if (p * p > n)
            break;
        if (n % p == 0) {
            spf = p;
            break;
        }
    }

    /* Return the index of the smallest prime factor. */
    return index_of_prime(spf);
}
