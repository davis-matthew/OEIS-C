/*
 * A050944  Numbers k such that sigma(k) > sigma(k+1) > sigma(k+2) >
 *          sigma(k+3).
 * https://oeis.org/A050944
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 528, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A050944
 *
 * Numbers k such that σ(k) > σ(k+1) > σ(k+2) > σ(k+3).
 * The function uses a dynamic sieve to compute divisor sums and
 * records the qualifying k-values.  It keeps a static cache of
 * previously found terms so that calls may arrive in any order.
 * The sieve size is increased geometrically until the requested
 * number of terms has been found.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* Minimum initial limit for the sieve (k up to this value will be examined). */
#define INITIAL_MAX_K 1000

/* Forward declaration of helper functions */
static void recompute_sigma(size_t max_k);

/* Static mutable state used by the function */
static seq_t *sigma = NULL;        /* sigma[i] = σ(i)  (i = 0..max_k+3)  */
static size_t sigma_size = 0;      /* current size of sigma array */
static seq_t *found = NULL;        /* array of found k-values */
static size_t found_len = 0;       /* number of stored k-values */
static size_t found_cap = 0;       /* capacity of the found array */
static size_t max_k = 0;           /* largest k for which sigma is valid   */
static size_t last_scanned_k = 0;  /* last k that was checked for a qualifying run */

/* -------------------------------------------------------------------------- */
/*               Recompute the divisor-sum array up to the given k              */
/* -------------------------------------------------------------------------- */
static void recompute_sigma(size_t max_k)
{
    /* We need sigma values up to k+3, so allocate size max_k + 4 */
    size_t new_size = max_k + 4;
    seq_t *new_sigma = (seq_t *)realloc(sigma, new_size * sizeof(seq_t));
    sigma = new_sigma;
    /* Initialise to zero */
    for (size_t i = 0; i < new_size; ++i)
        sigma[i] = 0;
    /* Classic sieve for divisor sums */
    for (size_t i = 1; i < new_size; ++i) {
        seq_t iv = (seq_t)i;
        for (size_t j = i; j < new_size; j += i)
            sigma[j] += iv;
    }
    sigma_size = new_size;
}

/* -------------------------------------------------------------------------- */
/*                    Ensure that at least `target` terms are known              */
/* -------------------------------------------------------------------------- */
static void ensure_found(size_t target)
{
    while (found_len < target) {
        /* Expand the sieve range */
        size_t new_max_k = (max_k == 0) ? INITIAL_MAX_K : max_k * 2;
        recompute_sigma(new_max_k);
        /* Scan only the newly added range */
        for (size_t k = last_scanned_k + 1; k + 3 <= new_max_k; ++k) {
            if (sigma[k] > sigma[k + 1] &&
                sigma[k + 1] > sigma[k + 2] &&
                sigma[k + 2] > sigma[k + 3]) {
                /* Store the qualifying k */
                if (found_len == found_cap) {
                    found_cap = (found_cap == 0) ? 64u : found_cap * 2u;
                    found = (seq_t *)realloc(found, found_cap * sizeof(seq_t));
                }
                found[found_len++] = (seq_t)k;
            }
        }
        last_scanned_k = new_max_k - 3;
        max_k = new_max_k;
    }
}

/* -------------------------------------------------------------------------- */
/*                               Public API                                      */
/* -------------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0)     /* offset is 1; behaviour for n < 1 is irrelevant */
        return 0;
    ensure_found((size_t)n);
    return found[(size_t)n - 1];
}
