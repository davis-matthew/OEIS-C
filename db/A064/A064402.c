/*
 * A064402  Numbers k such that prime(k) + k is prime, where prime(k) is the
 *          k-th prime.
 * https://oeis.org/A064402
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the nth positive integer k such that prime(k)+k is prime
 * Range:     u8 n <= 4, u16 n <= 251, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   xmalloc, xrealloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation uses a simple sieve to generate primes and then tests the
   condition for each k.  The result is cached so repeated calls are fast. */

#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include "../utils/xmalloc.h"
#include "../utils/xrealloc.h"

/* Parameters: sieve limit must cover enough primes for the requested n.
   For n up to 10,000 a bound of 5 000 000 is comfortably large. */
#define SIEVE_LIMIT 5000000

/* static state */
static bool *isPrime = NULL;      /* primality flags */
static seq_t *prime_list = NULL;  /* list of primes */
static size_t prime_cap = 0, prime_len = 0;

static seq_t *values = NULL;      /* a(n) values computed so far */
static size_t val_cap = 0, val_len = 0;
static idx_t current_k = 0;       /* last k evaluated */

static bool initialized = false;

/* Ensure values array has room for at least new_cap elements */
static void ensure_val_cap(size_t new_cap)
{
    if (val_cap < new_cap) {
        size_t grown = val_cap ? val_cap * 2 : 1024;
        if (grown < new_cap) grown = new_cap;
        seq_t *tmp = (seq_t *)xrealloc(values, grown * sizeof(seq_t));
        if (!tmp) abort();
        values = tmp;
        val_cap = grown;
    }
}

/* Ensure prime list array has room for at least new_cap elements */
static void ensure_prime_cap(size_t new_cap)
{
    if (prime_cap < new_cap) {
        size_t grown = prime_cap ? prime_cap * 2 : 1024;
        if (grown < new_cap) grown = new_cap;
        seq_t *tmp = (seq_t *)xrealloc(prime_list, grown * sizeof(seq_t));
        if (!tmp) abort();
        prime_list = tmp;
        prime_cap = grown;
    }
}

/* One‑time initialization: sieve primes up to SIEVE_LIMIT */
static void init(void)
{
    /* Allocate primality flag array */
    isPrime = (bool *)xmalloc((SIEVE_LIMIT + 1) * sizeof(bool));
    if (!isPrime) abort();
    for (idx_t i = 0; i <= SIEVE_LIMIT; ++i) isPrime[i] = true;
    isPrime[0] = isPrime[1] = false;

    /* Simple Eratosthenes sieve */
    idx_t lim_sqrt = (idx_t)(sqrt((double)SIEVE_LIMIT) + 1.0);
    for (idx_t i = 2; i <= lim_sqrt; ++i) {
        if (isPrime[i]) {
            for (idx_t j = i * i; j <= SIEVE_LIMIT; j += i) {
                isPrime[j] = false;
            }
        }
    }

    /* Build prime list */
    prime_cap = 1024;
    prime_list = (seq_t *)xmalloc(prime_cap * sizeof(seq_t));
    if (!prime_list) abort();
    prime_len = 0;
    for (idx_t i = 2; i <= SIEVE_LIMIT; ++i) {
        if (isPrime[i]) {
            if (prime_len == prime_cap) ensure_prime_cap(prime_cap * 2);
            prime_list[prime_len++] = (seq_t)i;
        }
    }

    /* Prepare values cache */
    val_cap = 1024;
    values = (seq_t *)xmalloc(val_cap * sizeof(seq_t));
    if (!values) abort();
    val_len = 0;

    initialized = true;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (!initialized) init();

    if (n == 0) return 0;          /* undefined for n < 1 */

    /* Ensure we have computed at least n terms */
    while (val_len < n) {
        /* Advance to next k */
        ++current_k;
        if (current_k > prime_len) {
            /* Should never happen with sieve limit large enough */
            break;
        }

        /* prime(k) is prime_list[current_k-1] */
        seq_t pk = prime_list[current_k - 1];
        seq_t cand = pk + (seq_t)current_k;   /* candidate pk + k */

        /* Test if cand is prime (cand <= SIEVE_LIMIT assumed) */
        if (cand <= (seq_t)SIEVE_LIMIT && isPrime[(idx_t)cand]) {
            ensure_val_cap(val_len + 1);
            values[val_len++] = current_k;
        }
    }

    return (val_len >= n) ? values[n - 1] : (seq_t)0;
}
