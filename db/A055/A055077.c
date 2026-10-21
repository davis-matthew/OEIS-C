/*
 * A055077  Multiplicity of Max{gcd(d, n!/d)} when d runs over the divisors of
 *          n!.
 * https://oeis.org/A055077
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2^k, where k = # of primes p ≤ n such that the exponent
 *            of p * in n! is odd. Equivalently, k is the number of primes
 *            with an odd * sum floor(n/p^j). This is the definition of
 *            A055077. The function * below implements this directly, keeping
 *            a memoised list of primes up to * the largest n req...
 * Range:     u8 n <= 32, u16 n <= 59, u32 n <= 59, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* Static containers for prime numbers.  They are grown as larger n's
 * are requested. */
static uint32_t *prime_list = NULL;     /* array of primes */
static size_t   prime_cnt  = 0;         /* number of primes stored      */
static size_t   prime_cap  = 0;         /* allocated capacity           */
static uint32_t sieve_limit = 0;        /* largest n for which primes
                                          * are known.                     */

/* Append a new prime to the static list. */
static void push_prime(uint32_t p)
{
    if (prime_cnt == prime_cap) {
        prime_cap = prime_cap ? prime_cap * 2 : 64;
        uint32_t *tmp = (uint32_t *)realloc(prime_list, prime_cap * sizeof(uint32_t));
        if (!tmp) abort();                /* allocation failure */
        prime_list = tmp;
    }
    prime_list[prime_cnt++] = p;
}

/* Expands the prime list so that all primes ≤ limit are stored.  Uses a
 * simple Eratosthenes sieve for efficiency.  The sieve array is allocated
 * on the heap and freed before the function returns. */
static void ensure_primes(uint32_t limit)
{
    if (limit <= sieve_limit) return;   /* already ready */

    uint8_t *mark = (uint8_t *)calloc(limit + 1, 1);
    if (!mark) abort();

    for (uint32_t i = 2; (uint64_t)i * i <= limit; ++i) {
        if (!mark[i]) {
            for (uint32_t j = i * i; j <= limit; j += i) mark[j] = 1;
        }
    }
    prime_cnt = 0;
    for (uint32_t i = 2; i <= limit; ++i)
        if (!mark[i]) push_prime(i);

    free(mark);
    sieve_limit = limit;
}

/* External function required by the framework. */
seq_t a(idx_t n)
{
    /* the sequence is defined for n ≥ 1; a(1) = 1 is a special case. */
    if (n < 2) return (seq_t)1;

    /* obtain all primes up to n. */
    ensure_primes((uint32_t)n);

    unsigned int odd_count = 0;   /* number of primes with odd exponent */

    for (size_t i = 0; i < prime_cnt && prime_list[i] <= (uint32_t)n; ++i) {
        uint32_t p = prime_list[i];
        unsigned int parity = 0;   /* 0 = even, 1 = odd */

        /* compute exponent of p in n! by summing ⌊n/p^k⌋.  The
         * parity of the sum is updated by XORing with the parity of each
         * term (whether ⌊n/p^k⌋ is odd). */
        uint64_t d = p;
        while (d <= n) {
            parity ^= (unsigned)((n / d) & 1U);
            d *= p;
        }
        if (parity) ++odd_count;
    }

    /* The answer is 2^odd_count.  For arbitrary‑precision types the
     * loop below is safe; for 64‑bit values it also works and is fast
     * enough for the required n-range. */
    seq_t result = (seq_t)1;
    for (unsigned int i = 0; i < odd_count; ++i) result *= (seq_t)2;
    return result;
}
