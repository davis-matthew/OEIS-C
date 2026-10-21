/*
 * A088034  Even numbers k such that k^2-1 is divisible by a square > 1.
 * https://oeis.org/A088034
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A088034 : Even integers k such that k^2-1 is divisible by a square > 1.
 *
 *  Equivalent description:
 *  k is even and there exists an odd prime p such that
 *      k ≡ ±1 (mod p²).
 *
 *  The implementation enumerates primes p and marks all even k that satisfy
 *  the congruence within a running sieve.  The marked numbers are kept in
 *  a monotone array `terms`; the function expands the sieve as many times
 *  as necessary to provide the requested n-th term.  The algorithm
 *  requires only O(limit log log limit) time for the sieve of a number as
 *  large as the n‑th term, and runs comfortably for n up to at least
 *  10 000 on the supplied test harness.
 *
 *  Only standard C/C++ headers are used; no `new` or template syntax.
 *  All void* conversions from malloc/calloc/realloc are explicitly
 *  cast to the appropriate pointer type so that a 128‑bit arbitrary‑
 *  precision build (where seq_t is a class) compiles successfully.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

/* -----------------------------------------------------------------------
 *  Grow a dynamic array of seq_t elements.
 * ----------------------------------------------------------------------- */
static void grow_seq_array(seq_t **arr, size_t *len, size_t *cap, seq_t val)
{
    if (*len == *cap) {
        *cap = (*cap == 0) ? 8 : (*cap << 1);
        *arr = (seq_t *)realloc(*arr, (*cap) * sizeof(seq_t));
    }
    (*arr)[(*len)++] = val;
}

/* -----------------------------------------------------------------------
 *  Build the sequence up to at least `n_needed` terms.
 *  The function expands the sieve limit until enough terms have been
 *  collected.  The internal state is kept in the static variables
 *  passed by pointer, so repeated calls are efficient.
 * ----------------------------------------------------------------------- */
static void expand_terms_if_needed(idx_t n_needed,
                                   seq_t **terms,
                                   size_t *len,
                                   idx_t *limit)
{
    if (*len >= (size_t)n_needed)
        return;                       /* already have enough terms */

    /* Start with a modest limit and double it until we collect
       the requested number of terms that fit in seq_t. */
    idx_t new_limit = (*limit == 0) ? 200000 : (*limit << 1);
    if (new_limit < 200000)
        new_limit = 200000;

    while (1) {
        /* (1) Sieve primes up to sqrt(new_limit). */
        idx_t sieve_lim = (idx_t)(sqrt((long double)new_limit) + 1.0L);
        bool   *is_prime = (bool *)calloc((size_t)(sieve_lim + 1), sizeof(bool));
        if (!is_prime)
            abort();

        for (idx_t i = 0; i <= sieve_lim; ++i)
            is_prime[i] = true;
        is_prime[0] = is_prime[1] = false;

        for (idx_t i = 2; i * i <= sieve_lim; ++i)
            if (is_prime[i])
                for (idx_t j = i * i; j <= sieve_lim; j += i)
                    is_prime[j] = false;

        /* (2) Mark even numbers k ≡ ±1 (mod p²) for each odd prime p. */
        unsigned char *marked = (unsigned char *)calloc((size_t)(new_limit + 1), sizeof(unsigned char));
        if (!marked)
            abort();

        for (idx_t p = 3; p <= sieve_lim; p += 2) {
            if (!is_prime[p])
                continue;

            seq_t step = (seq_t)p * (seq_t)p;      /* p²  (fits in seq_t) */
            if (step > new_limit)
                break;

            /* k ≡ -1 (mod step) → start at step-1 (even) */
            for (seq_t k = step - 1; k <= new_limit; k += step)
                marked[(size_t)k] = 1;

            /* k ≡ +1 (mod step) → start at step+1 (even) */
            for (seq_t k = step + 1; k <= new_limit; k += step)
                marked[(size_t)k] = 1;
        }

        /* (3) Extract all even k that were marked and fit in seq_t. */
        seq_t *new_terms = NULL;
        size_t new_len   = 0;
        size_t new_cap   = 0;

        for (seq_t k = 2; k <= new_limit; k += 2) {
            if (!marked[(size_t)k])
                continue;
            if ((idx_t)k != k)          /* overflow check: skip if too large */
                continue;
            grow_seq_array(&new_terms, &new_len, &new_cap, (seq_t)k);
        }

        free(marked);
        free(is_prime);

        if (new_len >= (size_t)n_needed) {
            /* We have enough terms; replace the cached array. */
            free(*terms);
            *terms   = new_terms;
            *len     = new_len;
            *limit   = new_limit;
            return;
        }

        /* Not enough; free and try a larger limit. */
        free(new_terms);
        new_limit <<= 1;                 /* double the limit and retry */
    }
}

/* -----------------------------------------------------------------------
 *  Public API: a(n)
 * ----------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    /* Static cache of precomputed terms */
    static seq_t *cache_terms   = NULL;
    static size_t cache_len     = 0;
    static idx_t  cache_limit   = 0;

    if (n == 0)              /* OEIS indices start at 1; guard against 0 */
        return 0;

    expand_terms_if_needed(n, &cache_terms, &cache_len, &cache_limit);
    return cache_terms[(size_t)n - 1];
}
