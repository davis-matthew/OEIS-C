/*
 * A144574  Largest prime < nonprime(prime(n)).
 * https://oeis.org/A144574
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = largest prime < A005218[prime(n)]
 * Range:     u8 n <= 46, u16 n <= 57, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute the nth term of OEIS A144574.                     */
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>

/* Static state ----------------------------------------------------- */
static uint32_t *primes        = NULL;   /* primes list  */
static size_t   prime_count    = 0;     /* number of primes cached   */
static size_t   sieve_limit    = 0;     /* maximum sieved value so far */

static uint8_t *is_prime_arr   = NULL;   /* prime flags  */

static seq_t  *nonprimes       = NULL;   /* non‑prime list (A005218)   */
static size_t  nonprime_count = 0;       /* number of non‑primes cached */

/* ------------------------------------------------------------------ */
/* Recompute a full sieve up to the given upper bound.  All cached
 * data (prime list, nonprime list, is_prime flag array) are rebuilt. */
static void
ensure_sieve(size_t limit)
{
    if (limit <= sieve_limit)
        return;                              /* nothing to do */

    /* new sieve array */
    uint8_t *new_is = (uint8_t *)malloc(limit + 1);
    for (size_t i = 0; i <= limit; ++i)
        new_is[i] = 1;
    new_is[0] = new_is[1] = 0;

    for (size_t i = 2; i * i <= limit; ++i)
        if (new_is[i])
            for (size_t j = i * i; j <= limit; j += i)
                new_is[j] = 0;

    free(is_prime_arr);
    is_prime_arr = new_is;
    sieve_limit = limit;

    /* rebuild prime list */
    size_t pc = 0;
    for (size_t i = 2; i <= limit; ++i)
        if (is_prime_arr[i])
            ++pc;
    uint32_t *new_primes = (uint32_t *)malloc(pc * sizeof(uint32_t));
    size_t pidx = 0;
    for (size_t i = 2; i <= limit; ++i)
        if (is_prime_arr[i])
            new_primes[pidx++] = (uint32_t)i;
    free(primes);
    primes = new_primes;
    prime_count = pc;

    /* rebuild nonprime list (sequence A005218) */
    size_t nc = 0;
    for (size_t i = 1; i <= limit; ++i)
        if (!is_prime_arr[i])
            ++nc;
    seq_t *new_nonpr = (seq_t *)malloc(nc * sizeof(seq_t));
    size_t nidx = 0;
    for (size_t i = 1; i <= limit; ++i)
        if (!is_prime_arr[i])
            new_nonpr[nidx++] = (seq_t)i;
    free(nonprimes);
    nonprimes = new_nonpr;
    nonprime_count = nc;
}

/* ------------------------------------------------------------------ */
/* Return the n‑th term of sequence A144574 (n >= 1). */
seq_t
a(idx_t n)
{
    /* Ensure we have at least n primes cached. */
    while (prime_count < (size_t)n) {
        size_t new_limit = (sieve_limit == 0 ? 2 : sieve_limit * 2);
        ensure_sieve(new_limit);
    }

    /* nth prime (primes array is 0‑based). */
    idx_t p = (idx_t)primes[(size_t)(n - 1)];

    /* Ensure we have at least p non‑primes cached. */
    while (nonprime_count < (size_t)p) {
        size_t new_limit = (sieve_limit == 0 ? 2 : sieve_limit * 2);
        ensure_sieve(new_limit);
    }

    /* p‑th non‑prime value. */
    seq_t m = nonprimes[(size_t)(p - 1)];

    /* Search downward from m‑1 for the largest prime. */
    idx_t candidate = (idx_t)m - 1;
    while (candidate >= 2 && !is_prime_arr[(size_t)candidate])
        --candidate;

    return (seq_t)candidate;
}
