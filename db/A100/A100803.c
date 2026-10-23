/*
 * A100803  A100802(m) where A100802(m) > A100802(m-1).
 * https://oeis.org/A100803
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*(prime(n+1)-prime(n))-1 (OEIS A100803)
 * Range:     u8 n <= 84, u16 n <= 84, u32 n <= 84, u64 n <= 84, u128 n <= 84,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <math.h>

/* Prime storage ---------------------------------------------------------- */
static idx_t prime_count = 0;   /* number of primes stored */
static seq_t *prime_list = NULL;/* array of primes, 0‑based: prime_list[0] = 2 */
static idx_t sieve_limit = 0;   /* current upper bound of the sieve */

/* Ensure that prime_list[0..need] (i.e. need+1 primes) is available. */
static void ensure_primes(idx_t need)
{
    if (prime_count > need)                /* already enough */
        return;

    /* ------------------------------------------------------------------ */
    /* Estimate a limit for the (need+1)-th prime.  For k >= 6,
       p_k < k(ln k + ln ln k); a small safety margin is added.          */
    /* ------------------------------------------------------------------ */
    size_t k = (size_t)need + 1;
    size_t estimate;
    if (k < 6)
        estimate = 15;
    else {
        double dk = (double)k;
        estimate = (size_t)(dk * (log(dk) + log(log(dk)))) + 3;
    }

    /* Grow at least twice the previous limit to keep the number of sieves
       low.  (If we have no previous limit, start with a small one.)       */
    if (estimate < sieve_limit * 2)
        estimate = (sieve_limit == 0) ? 15 : sieve_limit * 2;

    /* ------------------------------------------------------------------ */
    /* Perform a classic sieve of Eratosthenes up to 'estimate'.         */
    /* ------------------------------------------------------------------ */
    unsigned char *is_composite = (unsigned char *)calloc(estimate + 1, 1);
    if (!is_composite)           /* out of memory – best effort */
        return;

    for (idx_t i = 2; i * i <= estimate; ++i) {
        if (!is_composite[i]) {
            for (idx_t j = i * i; j <= estimate; j += i)
                is_composite[j] = 1;
        }
    }

    /* Allocate/resize the prime array to hold the found primes. */
    /* A simple over‑estimate of the array size is used to avoid many
       reallocs during the fill loop.                                     */
    size_t proposed_capacity = estimate / 10 + 100; /* enough for p_k ~ k ln k */
    seq_t *new_list = (seq_t *)realloc(prime_list,
                                       proposed_capacity * sizeof(seq_t));
    if (!new_list) {
        free(is_composite);
        return;
    }
    prime_list = new_list;

    /* Fill the array with primes.  We intentionally do not shrink the
       array after the fill; the over‑estimate keeps the allocation
       efficient.                                                      */
    idx_t cnt = 0;
    for (idx_t i = 2; i <= estimate && cnt < proposed_capacity; ++i) {
        if (!is_composite[i]) {
            prime_list[cnt++] = (seq_t)i;
        }
    }

    free(is_composite);
    prime_count = cnt;
    sieve_limit = estimate;
}

/* --------------------------------------------------------------------- */
/* Public sequence function                                            */
/* --------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0)                     /* the sequence is 1‑indexed   */
        return (seq_t)0;

    ensure_primes(n);               /* we need prime(n) and prime(n+1) */
    if (prime_count <= n)           /* unlikely, but safe fallback */
        return (seq_t)0;

    seq_t gap = prime_list[n] - prime_list[n - 1];
    return gap * 2 - 1;
}
