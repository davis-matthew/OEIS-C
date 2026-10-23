/*
 * A022837  a(n) = c(0)*p(0) + ... + c(n)*p(n), where c(i) = 1 if a(i-1) <
 *          p(i) and c(i) = -1 if a(i-1) >= p(i) (p(0) = 1, p(i) = prime(i)).
 * https://oeis.org/A022837
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 34, u16 n <= 3593, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A022837
   a(0) = 1; for n>0
   a(n) = a(n-1) + prime(n)   if a(n-1) < prime(n)
          a(n-1) - prime(n)   otherwise
   where prime(0)=1, prime(1)=2, prime(i)=ith prime for i≥1.  */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* forward declaration of helper */
static void _ensure_primes(idx_t n);

/* static storage for the sequence and prime values */
static seq_t  *a_cache   = NULL;   /* a values */
static size_t  a_len     = 0;       /* number of a values stored */
static uint32_t *primes  = NULL;   /* prime(i) at index i (0..prime_len-1) */
static size_t  prime_len  = 0;     /* number of primes stored */
static uint32_t current_candidate = 3;  /* next number to test for prime */

/* generate primes up to index n (inclusive) */
static void _ensure_primes(idx_t n)
{
    if (prime_len == 0) {                /* first call: initialise */
        prime_len = 2;
        primes = (uint32_t *)malloc(prime_len * sizeof(uint32_t));
        primes[0] = 1;                   /* prime(0) = 1 */
        primes[1] = 2;                   /* prime(1) = 2 */
        current_candidate = 3;
    }

    /* extend prime list until index n is available */
    while (prime_len <= (size_t)n) {
        uint32_t candidate = current_candidate;
        bool is_prime = true;

        /* trial division by primes already found */
        for (size_t j = 1; j < prime_len; ++j) {
            uint32_t p = primes[j];
            if ((uint64_t)p * (uint64_t)p > candidate) break;
            if (candidate % p == 0) {
                is_prime = false;
                break;
            }
        }

        if (is_prime) {
            uint32_t *tmp = (uint32_t *)realloc(primes,
                                                (prime_len + 1) * sizeof(uint32_t));
            if (!tmp) abort();            /* allocation failure */
            primes = tmp;
            primes[prime_len] = candidate;
            ++prime_len;
        }
        ++current_candidate;
    }
}

/* main sequence function */
seq_t a(idx_t n)
{
    size_t sn = (size_t)n;          /* cast once for array indexing */

    _ensure_primes(n);               /* make sure needed prime available */

    /* extend a_cache up to n if necessary */
    if (a_len <= sn) {
        size_t newlen = sn + 1;
        seq_t *tmp = (seq_t *)realloc(a_cache, newlen * sizeof(seq_t));
        if (!tmp) abort();            /* allocation failure */
        a_cache = tmp;

        if (a_len == 0) {            /* initialise a(0) */
            a_cache[0] = 1;
            a_len = 1;
        }

        for (size_t i = a_len; i <= sn; ++i) {
            seq_t prev = a_cache[i - 1];
            seq_t p    = (seq_t)primes[i];
            a_cache[i] = (prev < p) ? (prev + p) : (prev - p);
        }
        a_len = newlen;
    }

    return a_cache[sn];
}
