/*
 * A069209  Orders of non-Abelian Z-groups.
 * https://oeis.org/A069209
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 144, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A069209 – integers n that have at least two distinct prime factors p, q
   with p | (q-1).  The terms are listed in increasing order; a(1)=6, a(2)=10, … */

#include <stdlib.h>
#include <math.h>
#include <string.h>

/* Terminology:
   idx_t  : unsigned integer type for indices (given by the build).
   seq_t  : unsigned integer type used for sequence values (given by the build). */

static idx_t *prime_list      = NULL;   /* list of primes */
static size_t prime_count      = 0;      /* number of primes stored   */
static idx_t  max_prime_limit  = 0;      /* highest prime precomputed */

static seq_t *term_list       = NULL;   /* cached sequence terms     */
static size_t term_count       = 0;      /* number of terms stored   */
static size_t term_cap         = 0;      /* allocated capacity       */

static idx_t  last_candidate   = 0;      /* last integer examined    */

/* Generate all primes up to ‘limit’ (inclusive) and store them in prime_list. */
static void ensure_primes_upto(idx_t limit)
{
    if (limit <= max_prime_limit)
        return;    /* already have enough primes */

    free(prime_list);          /* discard old list */
    prime_list = NULL;
    prime_count = 0;
    max_prime_limit = 0;

    idx_t size = limit + 1;    /* array of flags */
    char *is_prime = (char *)malloc((size_t)size);
    if (!is_prime)
        exit(1);

    memset(is_prime, 1, (size_t)size);
    if (size > 1) {
        is_prime[0] = is_prime[1] = 0;
    }

    for (idx_t p = 2; p * p <= limit; ++p) {
        if (is_prime[p]) {
            for (idx_t j = p * p; j <= limit; j += p)
                is_prime[j] = 0;
        }
    }

    /* Count primes */
    for (idx_t i = 2; i <= limit; ++i)
        if (is_prime[i])
            ++prime_count;

    prime_list = (idx_t *)malloc(prime_count * sizeof(idx_t));
    if (!prime_list)
        exit(1);

    size_t idx = 0;
    for (idx_t i = 2; i <= limit; ++i)
        if (is_prime[i])
            prime_list[idx++] = i;

    free(is_prime);
    max_prime_limit = limit;
}

/* Check whether x satisfies the “Z‑group” condition:
   at least two distinct prime factors p,q with p | (q-1). */
static int qualifies(idx_t x)
{
    if (x < 6)        /* smallest number in the sequence is 6 */
        return 0;

    /* factor x keeping only distinct primes */
    idx_t factors[64];
    size_t fcount = 0;
    idx_t t = x;

    /* ensure primes large enough for factorisation */
    idx_t need = (idx_t)(sqrt((double)t)) + 1;
    ensure_primes_upto(need);

    for (size_t i = 0; i < prime_count && prime_list[i] * prime_list[i] <= t; ++i) {
        idx_t p = prime_list[i];
        if (t % p == 0) {
            factors[fcount++] = p;
            while (t % p == 0)
                t /= p;
        }
    }
    if (t > 1)            /* remaining prime factor > sqrt(x) */
        factors[fcount++] = t;

    if (fcount < 2)
        return 0;

    /* test all pairs */
    for (size_t i = 0; i + 1 < fcount; ++i) {
        for (size_t j = i + 1; j < fcount; ++j) {
            if ((factors[j] - 1) % factors[i] == 0 || (factors[i] - 1) % factors[j] == 0)
                return 1;
        }
    }
    return 0;
}

/* Store a new term into the cached array, enlarging it if needed. */
static void append_term(seq_t value)
{
    if (term_count == term_cap) {
        size_t new_cap = term_cap == 0 ? 32 : term_cap * 2;
        seq_t *new_arr = (seq_t *)realloc(term_list, new_cap * sizeof(seq_t));
        if (!new_arr)
            exit(1);
        term_list = new_arr;
        term_cap = new_cap;
    }
    term_list[term_count++] = value;
}

/* Main function required by the evaluation framework. */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;          /* undefined for n<1, but harmless */

    /* Generate terms until we have at least n of them. */
    while (term_count < n) {
        ++last_candidate;
        idx_t sqrt_needed = (idx_t)(sqrt((double)last_candidate)) + 1;
        ensure_primes_upto(sqrt_needed);
        if (qualifies(last_candidate))
            append_term((seq_t)last_candidate);
    }
    return term_list[n - 1];
}
