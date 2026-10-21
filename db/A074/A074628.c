/*
 * A074628  Numbers k such that sigma(k) == 2 mod 6.
 * https://oeis.org/A074628
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 53, u16 n <= 8445, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Sequence A074628: numbers k such that σ(k) == 2 (mod 6)
 * This implementation pre‑computes σ(k) modulo 6 with a divisor sieve
 * and stores all k that satisfy the congruence.  The array of found
 * terms is extended as needed to answer any query a(n), n ≥ 1.
 * The algorithm is O(N log N) in the largest value of k that has
 * been examined, which is well below the limits for n ≈ 10 000.
 */

#include <stdlib.h>       /* calloc, realloc, free, exit */
#include <stddef.h>       /* size_t */
#include <stdint.h>       /* uint64_t (idx_t) */

/* Forward declarations of helpers */
static void recompute_sigma(idx_t limit);
static void recompute_terms(idx_t limit);

/* Static state for the sieve and the list of terms */
static idx_t current_limit = 0;          /* maximum k for which sigma has been computed */
static unsigned char *sigma_mod6 = NULL; /* sigma(k) modulo 6 for 1 ≤ k ≤ current_limit */
static size_t sigma_alloc = 0;           /* allocated size of sigma_mod6 */

static idx_t *terms = NULL;              /* array of k that satisfy the condition */
static size_t terms_len = 0;             /* number of stored terms */
static size_t terms_cap = 0;             /* capacity of terms */

/* Compute the n-th term (n ≥ 1). */
seq_t a(idx_t n)
{
    /* If the request is outside the defined offset, return 0. */
    if (n == 0)
        return (seq_t)0;

    /* Generate more terms while we don't have enough. */
    while (terms_len < n) {
        /* Choose a new limit.  Start at 256 and double each time. */
        idx_t new_limit = (current_limit > 0) ? current_limit * 2 + 1 : 256;
        recompute_sigma(new_limit);
        recompute_terms(new_limit);
    }

    /* Return the requested term (1‑based indexing). */
    return (seq_t)terms[n - 1];
}

/*
 * Recompute the array sigma_mod6 up to 'limit'.
 * sigma_mod6[k] holds σ(k) modulo 6 for 1 ≤ k ≤ limit.
 */
static void recompute_sigma(idx_t limit)
{
    size_t sz = (size_t)limit + 1;          /* include index 0 */
    unsigned char *new_sigma = (unsigned char*)calloc(sz, sizeof(unsigned char));
    if (!new_sigma)
        exit(EXIT_FAILURE);               /* allocation failed */

    /* Divisor sieve: add i modulo 6 to all multiples of i. */
    for (idx_t i = 1; i <= limit; ++i) {
        unsigned char imod = (unsigned char)(i % 6);
        for (idx_t j = i; j <= limit; j += i) {
            new_sigma[(size_t)j] += imod;
            if (new_sigma[(size_t)j] >= 6)
                new_sigma[(size_t)j] -= 6;
        }
    }

    /* Free old array and update global pointers. */
    free(sigma_mod6);
    sigma_mod6  = new_sigma;
    sigma_alloc = sz;
    current_limit = limit;
}

/*
 * Rebuild the list of terms up to 'limit'.
 * All k with sigma_mod6[k] == 2 are stored in the global 'terms' array.
 */
static void recompute_terms(idx_t limit)
{
    /* Reset the list.  Allocate enough capacity for the whole range. */
    size_t new_cap = (size_t)limit;
    idx_t *new_terms = (idx_t*)realloc(terms, new_cap * sizeof(idx_t));
    if (!new_terms)
        exit(EXIT_FAILURE);
    terms     = new_terms;
    terms_cap = new_cap;
    terms_len = 0;

    /* Scan all k and collect those with σ(k) ≡ 2 (mod 6). */
    for (idx_t k = 1; k <= limit; ++k) {
        if (sigma_mod6[(size_t)k] == 2) {
            if (terms_len == terms_cap) {            /* should not happen */
                size_t new_capacity = terms_cap * 2 + 1;
                idx_t *tmp = (idx_t*)realloc(terms, new_capacity * sizeof(idx_t));
                if (!tmp)
                    exit(EXIT_FAILURE);
                terms     = tmp;
                terms_cap = new_capacity;
            }
            terms[terms_len++] = k;
        }
    }
}
