/*
 * A046320  Odd numbers divisible by exactly 7 primes (counted with
 *          multiplicity).
 * https://oeis.org/A046320
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   numbers that are odd and have exactly 7 prime factors, counted
 *            with multiplicity (i.e., ω(n)=7 and n is odd)
 * Range:     u8 none, u16 n <= 90, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   _smallest_prime_factor
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>

/* required helper */
#include "../utils/_smallest_prime_factor.h"

/* static state for memoisation */
static bool initialized = false;
static seq_t *terms = NULL;              /* array of found numbers    */
static size_t terms_len = 0;             /* number of terms stored   */
static size_t terms_cap = 0;             /* capacity of terms        */
static idx_t next_candidate = 3;         /* next odd number to test  */
static idx_t sieve_limit = 2000000;      /* length of pre‑computed sieve */
static uint32_t *spf = NULL;             /* smallest prime factor       */
static uint8_t *omega = NULL;            /* number of prime factors     */

/* grow the terms array when needed */
static void grow_terms(size_t needed)
{
    if (needed <= terms_cap)
        return;
    size_t new_cap = terms_cap ? terms_cap * 2 : 1024;
    while (new_cap < needed)
        new_cap *= 2;
    seq_t *new_terms = (seq_t *)realloc((void *)terms, new_cap * sizeof(seq_t));
    if (!new_terms)
        abort();            /* out of memory */
    terms = new_terms;
    terms_cap = new_cap;
}

/* compute omega(n) for n > sieve_limit using _smallest_prime_factor */
static uint8_t omega_larger(uint64_t n)
{
    uint8_t cnt = 0;
    while (n > 1) {
        uint64_t p = _smallest_prime_factor(n);
        n /= p;
        cnt++;
    }
    return cnt;
}

/* initialise the small sieve up to sieve_limit */
static void init_sieve(void)
{
    /* allocate spf array */
    spf = (uint32_t *)malloc((size_t)(sieve_limit + 1) * sizeof(uint32_t));
    if (!spf) abort();
    for (idx_t i = 0; i <= sieve_limit; ++i) spf[i] = 0;

    /* build spf using standard sieve */
    for (idx_t i = 2; i <= sieve_limit; ++i) {
        if (spf[i] == 0) {                /* i is prime */
            spf[i] = (uint32_t)i;
            if (i * i <= sieve_limit) {
                for (idx_t j = i * i; j <= sieve_limit; j += i)
                    if (spf[j] == 0) spf[j] = (uint32_t)i;
            }
        }
    }

    /* allocate omega array */
    omega = (uint8_t *)malloc((size_t)(sieve_limit + 1) * sizeof(uint8_t));
    if (!omega) abort();
    omega[1] = 0;
    for (idx_t i = 2; i <= sieve_limit; ++i)
        omega[i] = omega[i / spf[i]] + 1;
}

/* expand terms array to contain at least 'need' entries */
static void ensure_terms(idx_t need)
{
    if (terms_len >= (size_t)need)
        return;
    /* grow enough space */
    grow_terms((size_t)need);

    /* continue searching from next_candidate */
    while (terms_len < (size_t)need) {
        uint8_t cnt = (next_candidate <= sieve_limit) ?
                      omega[next_candidate] : omega_larger(next_candidate);
        if (cnt == 7) {
            terms[terms_len++] = (seq_t)next_candidate;
        }
        next_candidate += 2;            /* only odd numbers */
    }
}

/* main sequence function */
seq_t a(idx_t n)
{
    if (n == 0)          /* offset 1; undefined for n=0 */
        return (seq_t)0;

    if (!initialized) {
        init_sieve();
        terms_cap = 8000;            /* initial capacity */
        terms = (seq_t *)malloc(terms_cap * sizeof(seq_t));
        if (!terms) abort();
        terms_len = 0;
        /* pre‑populate with first terms up to a reasonable number */
        ensure_terms(10000);
        initialized = true;
    }
    /* ensure we have at least n terms */
    if ((idx_t)terms_len < n) {
        ensure_terms(n);
    }
    return terms[n - 1];
}
