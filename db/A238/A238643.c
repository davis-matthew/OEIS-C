/*
 * A238643  Number of primes p <= n such that 2*pi(p) - (-1)^n and p*n
 *          +((-1)^n - 3)/2 are both prime, where pi(x) is the number of
 *          primes not exceeding x.
 * https://oeis.org/A238643
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 19, u16 n <= 1055, u32 n <= 1055, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A238643
 *  a(n) = number of primes p <= n such that
 *         2*π(p) - (-1)^n   and   p*n + ((-1)^n - 3)/2
 *         are both prime.  (π(x) = number of primes <= x)
 *
 *  The function is fully self‑contained: it builds a small sieve up to
 *  the requested n and uses is_prime_big() for primality testing.
 *  Results are cached so repeated or out‑of‑order calls are fast.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/is_prime_big.h"

/* ---------- internal storage for sieve ---------- */
static idx_t   sieve_limit  = 0;          /* current maximum n processed   */
static uint8_t *is_prime_arr  = NULL;     /* 0/1 array of primality         */
static uint32_t *prime_prefix = NULL;     /* π(i) for i <= sieve_limit      */
static uint32_t *prime_list   = NULL;     /* list of primes up to limit     */
static size_t   prime_cnt     = 0;         /* number of primes stored        */

/* ---------- extend sieve to at least n ---------- */
static void extend_sieve(idx_t n)
{
    if (n <= sieve_limit)
        return;                      /* already up to date */

    /* Allocate (or resize) the arrays.  We may reallocate to a larger
     * size; if realloc fails, we keep the previous data which is
     * still usable for smaller n.  For the expected n <= 10000
     * this is safe and fast. */
    size_t new_size = (size_t)n + 1U;

    uint8_t  *new_prime = NULL;
    uint32_t *new_prefix = NULL;
    uint32_t *new_list = NULL;

    new_prime  = (uint8_t *)realloc(is_prime_arr,  new_size);
    new_prefix = (uint32_t *)realloc(prime_prefix, new_size * sizeof(uint32_t));
    new_list   = (uint32_t *)realloc(prime_list,   new_size * sizeof(uint32_t));

    if (!new_prime || !new_prefix || !new_list) {
        /* If allocation failed, fall back to the existing arrays. */
        if (new_prime)   free(new_prime);
        if (new_prefix)  free(new_prefix);
        if (new_list)    free(new_list);
        return;
    }

    is_prime_arr = new_prime;
    prime_prefix = new_prefix;
    prime_list   = new_list;

    /* initialise all entries as prime */
    for (size_t i = 0; i < new_size; ++i)
        is_prime_arr[i] = 1;
    if (n >= 0) is_prime_arr[0] = 0;
    if (n >= 1) is_prime_arr[1] = 0;

    /* Eratosthenes sieve */
    for (size_t i = 2; i * i <= (size_t)n; ++i)
        if (is_prime_arr[i])
            for (size_t j = i * i; j <= (size_t)n; j += i)
                is_prime_arr[j] = 0;

    /* build prefix counts and prime list */
    prime_cnt = 0;
    prime_prefix[0] = 0;
    for (size_t i = 1; i <= (size_t)n; ++i) {
        prime_prefix[i] = prime_prefix[i-1] + is_prime_arr[i];
        if (is_prime_arr[i])
            prime_list[prime_cnt++] = (uint32_t)i;
    }

    sieve_limit = n;
}

/* ---------- the requested sequence function ---------- */
seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;

    extend_sieve(n);

    seq_t count = 0;

    /* (-1)^n :  1 if n even, -1 if n odd   */
    int64_t neg_pow = (n & 1U) ? -1LL : 1LL;

    /* ((-1)^n - 3)/2 :   -2 if n odd, -1 if n even */
    int64_t term2_offset = (n & 1U) ? -2LL : -1LL;

    /* iterate over all primes p <= n */
    for (size_t i = 0; i < prime_cnt && prime_list[i] <= (uint32_t)n; ++i) {
        uint32_t p     = prime_list[i];
        uint32_t pi_p  = prime_prefix[p];                 /* π(p) */
        /* 2*π(p) - (-1)^n  */
        int64_t t1_val = 2LL * (int64_t)pi_p - neg_pow;
        if (!is_prime_big((seq_t)t1_val))
            continue;

        /* p*n + ((-1)^n - 3)/2  */
        int64_t prod = (int64_t)p * (int64_t)n + term2_offset;
        /* prod is always positive for p <= n >= 2 */
        if (!is_prime_big((seq_t)prod))
            continue;

        ++count;
    }
    return count;
}
