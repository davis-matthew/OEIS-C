/*
 * A163858  Number of sexy prime triples (p, p+6, p+12) where p+18 is not
 *          prime (although p-6 might be), with p <= n.
 * https://oeis.org/A163858
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 9919, u16 n <= 9919, u32 n <= 9919, u64 n <= 9919,
 *            u128 n <= 9919, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Compute a(n) = number of sexy prime triples (p, p+6, p+12) with p ≤ n
 * where p+18 is not prime.
 * Algorithm: sieve primes up to n+18 and count triples on the fly.
 * Memoized: static arrays keep results for repeated calls.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* static state */
static idx_t   max_n   = 0;     /* largest n processed so far */
static idx_t   sieve_lim = 0;   /* current sieve limit (p+18)   */
static bool   *is_prime = NULL;/* primality sieve up to sieve_lim */
static seq_t  *cumul    = NULL;/* cumulative counts 0..n */

/* Rebuild sieve and cumulative array up to required limit n. */
static void
rebuild(idx_t n)
{
    /* Sieve up to n + 18 to be able to check p+18. */
    idx_t limit = n + 18;

    /* Resize sieve if needed. */
    if (sieve_lim < limit) {
        size_t new_size = (size_t)(limit + 1);       /* indices 0..limit */
        bool *tmp = (bool *)realloc(is_prime, new_size);
        if (!tmp) exit(1);                          /* out of memory */
        is_prime = tmp;
        /* initialise all entries to true */
        for (size_t i = 0; i < new_size; ++i) is_prime[i] = true;
        if (limit >= 0) is_prime[0] = false;
        if (limit >= 1) is_prime[1] = false;
        for (idx_t p = 2; p * p <= limit; ++p) {
            if (is_prime[p]) {
                for (idx_t m = p * p; m <= limit; m += p)
                    is_prime[m] = false;
            }
        }
        sieve_lim = limit;
    }

    /* Resize cumulative array to hold entries up to n. */
    size_t new_csize = (size_t)(n + 1);          /* indices 0..n */
    seq_t *tmp_c = (seq_t *)realloc(cumul, new_csize * sizeof(seq_t));
    if (!tmp_c) exit(1);
    cumul = tmp_c;

    /* Compute cumulative counts up to n. */
    cumul[0] = 0;
    seq_t cnt = 0;
    for (idx_t i = 1; i <= n; ++i) {
        if (is_prime[i] && is_prime[i + 6] && is_prime[i + 12] && !is_prime[i + 18])
            ++cnt;
        cumul[i] = cnt;
    }
    max_n = n;
}

seq_t a(idx_t n)
{
    if (n < 1) return 0; /* a(0) not defined, return 0 for safety */
    if (n > max_n) rebuild(n);
    return cumul[n];
}
