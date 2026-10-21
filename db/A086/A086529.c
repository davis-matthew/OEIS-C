/*
 * A086529  Beginning with 2, distinct even numbers such that the arithmetic
 *          mean of successive pairs of terms gives odd primes in their
 *          natural order. {a(n) + a(n+1)}/2 = prime(n+1).
 * https://oeis.org/A086529
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 48, u16 n <= 58, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A086529
   a(1) = 2
   a(n) = 2 * p(n) - a(n-1)   (n ≥ 2)
   where p(n) is the nth prime (p(1)=2). */

/* The implementation generates primes on demand and evaluates
   a(n) by iterating from 2 to n. No term memoisation is needed. */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>

/* -------- Prime generation state -------- */
static idx_t       prime_cnt  = 0;   /* number of primes stored (p[1]..p[prime_cnt])   */
static size_t      prime_cap  = 0;   /* current capacity of primes array              */
static uint32_t   *primes     = NULL;/* primes[1]..primes[prime_cnt] (prime[1]=2)      */
static idx_t       next_candidate = 2;/* integer to test for primality                  */

/* Ensure primes array has space for at least 'need' primes. */
static void ensure_primes(idx_t need)
{
    /* Grow array storage if required */
    if (prime_cap < (size_t)need + 1) {
        size_t new_cap = prime_cap ? prime_cap * 2 : 16;
        if (new_cap < (size_t)need + 1) new_cap = need + 1;
        uint32_t *tmp = (uint32_t *)realloc(primes, new_cap * sizeof(uint32_t));
        if (!tmp) abort();
        primes = tmp;
        prime_cap = new_cap;
    }

    /* Generate primes until we have 'need' of them */
    while (prime_cnt < need) {
        bool isprime = true;
        for (size_t i = 1; i <= prime_cnt &&
                 (uint64_t)primes[i] * (uint64_t)primes[i] <= (uint64_t)next_candidate;
             ++i) {
            if (next_candidate % primes[i] == 0) {
                isprime = false;
                break;
            }
        }
        if (isprime) {
            ++prime_cnt;
            primes[prime_cnt] = (uint32_t)next_candidate;
        }
        ++next_candidate;
    }
}

/* -------- Sequence function -------- */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;      /* undefined for n<1 */

    /* Generate the necessary primes */
    ensure_primes(n);

    /* Compute a(n) iteratively using the recurrence */
    seq_t prev = (seq_t)2;           /* a(1) */
    if (n == 1) return prev;

    for (idx_t i = 2; i <= n; ++i) {
        seq_t prime_val = (seq_t)primes[(size_t)i];
        prev = (seq_t)2 * prime_val - prev;
    }
    return prev;
}
