/*
 * A235177  a(n) is a refactorable number and the sum of all refactorable
 *          numbers <= a(n) is also a refactorable number.
 * https://oeis.org/A235177
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 638, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) is the nth refactorable number such that the sum of all
 * refactorable numbers <= it is also a refactorable number.
 *
 * The algorithm maintains a running list of found terms, a cumulative
 * sum, and checks each integer in ascending order.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <limits.h>

/* global state ---------------------------------------------------------- */

static seq_t *terms      = NULL;   /* array of found terms         */
static size_t terms_size = 0;      /* number of terms found        */
static seq_t  cumulative = 0;      /* running sum of refactorables */

static idx_t last_checked = 0;     /* smallest integer not yet tested */

static uint32_t *prime_list = NULL;  /* primes up to 1'000'000      */
static size_t   prime_count = 0;

/* generate all primes up to 1,000,000 ----------------------------------- */

static void init_primes(void)
{
    if (prime_list != NULL)        /* already initialised            */
        return;

    const size_t LIMIT = 1000000;
    char *is_prime = (char *)malloc(LIMIT + 1);
    if (!is_prime) exit(EXIT_FAILURE);

    for (size_t i = 0; i <= LIMIT; ++i)
        is_prime[i] = 1;
    is_prime[0] = is_prime[1] = 0;

    for (size_t i = 2; i * i <= LIMIT; ++i) {
        if (is_prime[i]) {
            for (size_t j = i * i; j <= LIMIT; j += i)
                is_prime[j] = 0;
        }
    }

    /* count primes */
    size_t count = 0;
    for (size_t i = 2; i <= LIMIT; ++i)
        if (is_prime[i]) ++count;

    prime_list = (uint32_t *)malloc(count * sizeof(uint32_t));
    if (!prime_list) exit(EXIT_FAILURE);

    size_t idx = 0;
    for (size_t i = 2; i <= LIMIT; ++i)
        if (is_prime[i]) prime_list[idx++] = (uint32_t)i;

    prime_count = count;
    free(is_prime);
}

/* divisor count for a uint64_t ------------------------------------------ */

static uint32_t divisor_count_u64(uint64_t n)
{
    if (n <= 1)
        return 1;

    uint32_t dcount = 1;
    uint64_t rem = n;

    for (size_t i = 0; i < prime_count &&
             (uint64_t)prime_list[i] * prime_list[i] <= rem; ++i)
    {
        uint64_t p = prime_list[i];
        if (rem % p == 0) {
            uint32_t exp = 0;
            do {
                rem /= p;
                ++exp;
            } while (rem % p == 0);
            dcount *= (exp + 1);
        }
    }

    if (rem > 1)          /* remaining prime factor   */
        dcount *= 2;

    return dcount;
}

/* check whether a seq_t value is refactorable --------------------------- */

static bool is_refactorable(seq_t val, uint32_t *out_dcount)
{
    /* only numbers that fit in 64 bits can be tested */
    if (sizeof(seq_t) <= sizeof(uint64_t) || (uint64_t)val <= ULLONG_MAX) {
        uint64_t n = (uint64_t)val;
        uint32_t d = divisor_count_u64(n);
        if (out_dcount)
            *out_dcount = d;
        return (n % d) == 0;
    }
    return false;          /* too large – cannot factorise safely */
}

/* main sequence routine ----------------------------------------------- */

seq_t a(idx_t n)
{
    if (n == 0)               /* outside defined offset */
        return 0;

    init_primes();            /* ensure primes available */

    while (terms_size < (size_t)n) {
        ++last_checked;                   /* next integer to test */

        uint32_t d_candidate;
        if (!is_refactorable((seq_t)last_checked, &d_candidate))
            continue;                   /* not refactorable           */

        /* candidate is refactorable – update cumulative sum */
        cumulative += (seq_t)last_checked;

        /* check whether the new cumulative sum is refactorable */
        if (!is_refactorable(cumulative, NULL))
            continue;                   /* cumulative not refactorable */

        /* store the found term */
        seq_t *tmp = (seq_t *)realloc(terms, (terms_size + 1) * sizeof(seq_t));
        if (!tmp) exit(EXIT_FAILURE);
        terms = tmp;
        terms[terms_size++] = (seq_t)last_checked;
    }

    return terms[n - 1];
}
