/*
 * A090609  Primes of the form 6*p - 1 such that p and 6*p - 5 are primes.
 * https://oeis.org/A090609
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 44, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation of A090609:
 * a(n) = 6*p - 1 where p is the n‑th prime such that p, 6p-5 and 6p-1 are prime. */

/* The implementation caches previously found terms and primes.  On each
 * request it generates further terms until the desired index is reached.
 * Primality is tested with is_prime_big, which works for the seq_t type
 * supplied by the build.  No hard coded table of values is used. */

#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>

#include "../utils/is_prime_big.h"

/* Static state ----------------------------------------------------------- */
static seq_t *terms = NULL;           /* stored sequence values   */
static size_t terms_len = 0;
static size_t terms_cap = 0;

static idx_t *prime_list = NULL;     /* list of primes found so far */
static size_t prime_len = 0;
static size_t prime_cap = 0;
static idx_t next_prime_candidate = 3;   /* next odd number to test */

static idx_t current_prime_idx = 0;  /* index of the next prime to evaluate */

/* ------------------------------------------------------------------------ */

static void ensure_terms_capacity(size_t needed)
{
    if (terms_cap >= needed) return;
    size_t new_cap = (terms_cap == 0) ? 16 : terms_cap * 2;
    while (new_cap < needed) new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(terms, new_cap * sizeof(seq_t));
    if (!tmp) abort();
    terms = tmp;
    terms_cap = new_cap;
}

static void ensure_prime_capacity(size_t needed)
{
    if (prime_cap >= needed) return;
    size_t new_cap = (prime_cap == 0) ? 16 : prime_cap * 2;
    while (new_cap < needed) new_cap <<= 1;
    idx_t *tmp = (idx_t *)realloc(prime_list, new_cap * sizeof(idx_t));
    if (!tmp) abort();
    prime_list = tmp;
    prime_cap = new_cap;
}

/* Initialise the prime list with the first prime 2. */
static void init_primes(void)
{
    static bool already_done = false;
    if (already_done) return;
    already_done = true;

    ensure_prime_capacity(1);
    prime_list[0] = 2;
    prime_len = 1;
    next_prime_candidate = 3;          /* begin searching odd numbers */
}

/* Generate sequence terms until we have at least n_requested terms. */
static void generate_until(idx_t n_requested)
{
    init_primes();

    while (terms_len < n_requested) {
        /* Ensure we have a prime available for the current index. */
        if (current_prime_idx >= prime_len) {
            /* Generate the next prime and append it. */
            while (!is_prime_big((seq_t)next_prime_candidate))
                next_prime_candidate += 2;        /* skip even numbers */

            ensure_prime_capacity(prime_len + 1);
            prime_list[prime_len++] = next_prime_candidate;
            next_prime_candidate += 2;
        }

        /* Retrieve and advance the prime to be examined. */
        idx_t p = prime_list[current_prime_idx++];
        seq_t np5 = (seq_t)p * 6U - 5U;   /* 6p - 5 */
        seq_t np1 = (seq_t)p * 6U - 1U;   /* 6p - 1 */

        if (is_prime_big(np5) && is_prime_big(np1)) {
            /* Store the term. */
            ensure_terms_capacity(terms_len + 1);
            terms[terms_len++] = np1;
        }
    }
}

/* ------------------------------------------------------------------------ */
/* Public API ------------------------------------------------------------ */

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* offset is 1; behaviour for n<1 is irrelevant */
    generate_until(n);
    return terms[n - 1];
}
