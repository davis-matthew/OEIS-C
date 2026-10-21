/*
 * A078782  Nonprimes (A018252) with prime (A000040) subscripts.
 * https://oeis.org/A078782
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = A018252(A000040(n)) /* Nonprime whose index is the n‑th
 *            prime
 * Range:     u8 n <= 46, u16 n <= 57, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation builds a simple sieve on demand and
   uses it both to obtain the n‑th prime and to search for the
   k‑th nonprime via the relation: rank_nonprime(m) = m - π(m). */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* Static cache for the prime sieve */
static idx_t  sieve_limit = 0;          /* largest number sieved */
static uint8_t *sieve = NULL;          /* 0/1 array of primality */
static uint64_t *prime_list = NULL;    /* array of primes found */
static size_t   prime_count = 0;        /* number of primes in prime_list */

/* Ensure that the sieve contains all numbers up to 'limit'.
   Rebuilds the sieve if a larger limit is requested. */
static void ensure_sieve(idx_t limit)
{
    if (sieve_limit >= limit) return;

    /* Free old sieve and prime array if any */
    free(sieve);
    free(prime_list);

    /* Allocate new sieve array */
    sieve = (uint8_t *)malloc((size_t)(limit + 1));
    if (!sieve) exit(1);
    for (idx_t i = 0; i <= limit; ++i) sieve[i] = 1;
    sieve[0] = sieve[1] = 0;

    /* Sieve of Eratosthenes */
    idx_t i, j;
    for (i = 2; (i * i) <= limit; ++i) {
        if (sieve[i]) {
            for (j = i * i; j <= limit; j += i) sieve[j] = 0;
        }
    }

    /* Count primes to allocate list */
    size_t count = 0;
    for (i = 2; i <= limit; ++i) if (sieve[i]) ++count;

    prime_list = (uint64_t *)malloc(count * sizeof(uint64_t));
    if (!prime_list) exit(1);

    /* Store primes */
    size_t idx = 0;
    for (i = 2; i <= limit; ++i) if (sieve[i]) prime_list[idx++] = (uint64_t)i;

    prime_count = count;
    sieve_limit = limit;
}

/* Return the n-th prime (1‑based).  If n is beyond the current sieve
   range, the sieve is enlarged until enough primes are available. */
static uint64_t nth_prime(idx_t n)
{
    if (n == 0) return 0;          /* undefined, but not used */

    /* Start with a reasonable base limit */
    idx_t limit = (n <= 1) ? 16 : (idx_t)(n * 15);
    while (true) {
        ensure_sieve(limit);
        if (prime_count >= (size_t)n) return prime_list[(size_t)n - 1];
        limit *= 2;                /* double until enough primes */
    }
}

/* Binary search for π(x) using the prime_list array. */
static idx_t prime_pi(idx_t x)
{
    size_t lo = 0, hi = prime_count;
    while (lo < hi) {
        size_t mid = (lo + hi) >> 1;
        if (prime_list[mid] <= (uint64_t)x) lo = mid + 1;
        else hi = mid;
    }
    return (idx_t)lo;
}

/* Find the k-th nonprime using binary search:
   rank_nonprime(m) = m - π(m).  Return the smallest m such that
   m - π(m) = k. */
static idx_t kth_nonprime(idx_t k)
{
    if (k == 0) return 0;           /* undefined, but not used */

    /* Rough bounds: m is at least k and at most 2k+100 */
    idx_t low = k;
    idx_t high = (k << 1) + 100;    /* 2*k + 100 */

    ensure_sieve(high);             /* need primes up to high */

    while (low < high) {
        idx_t mid = low + ((high - low) >> 1);
        idx_t rank = mid - prime_pi(mid);
        if (rank < k) low = mid + 1;
        else high = mid;
    }
    return low;
}

/* The required sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;        /* offset is 1, behaviour for 0 is irrelevant */

    /* Step 1: obtain the n-th prime */
    uint64_t p = nth_prime(n);

    /* Step 2: find the p-th nonprime */
    idx_t m = kth_nonprime((idx_t)p);

    return (seq_t)m;
}
