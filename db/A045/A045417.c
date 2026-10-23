/*
 * A045417  Primes congruent to {0, 1, 3, 4, 6} mod 7.
 * https://oeis.org/A045417
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 36, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A045417: primes that are congruent to 0, 1, 3, 4, or 6 (mod 7) */
/* The function builds the primes on demand and caches them.  It works
   for all supported seq_t and idx_t types.  The implementation
   relies only on trial division and dynamic arrays. */

#include <stdlib.h>
#include <stdint.h>

static seq_t *all_primes   = NULL;   /* all primes found so far          */
static size_t all_len      = 0;
static size_t all_cap      = 0;

static seq_t *filtered     = NULL;   /* primes that hit the congruence  */
static size_t filt_len      = 0;
static size_t filt_cap      = 0;

static uint64_t next_candidate = 2;   /* next odd number to test       */

/* Grow a dynamic array of seq_t elements.  The cast ensures C++ safety. */
static void grow_seq(seq_t **arr, size_t *len, size_t *cap)
{
    if (*len >= *cap) {
        size_t new_cap = (*cap == 0) ? 16 : (*cap << 1);
        *arr = (seq_t *)realloc(*arr, new_cap * sizeof(seq_t));
        *cap = new_cap;
    }
}

/* Test whether x (uint64_t) is prime, using the primes stored so far. */
static int is_prime_candidate(uint64_t x)
{
    if (x < 2) return 0;
    for (size_t i = 0; i < all_len; ++i) {
        uint64_t p = (uint64_t)all_primes[i];   /* all primes are small   */
        if (p * p > x) break;
        if (x % p == 0) return 0;
    }
    return 1;
}

/* Ensure that the cache contains at least n filtered primes. */
static void ensure_filtered(idx_t n)
{
    if (filt_len >= (size_t)n) return;

    /* Initialise arrays if not already. */
    if (all_cap == 0) {
        all_primes = (seq_t *)malloc(16 * sizeof(seq_t));
        all_cap = 16;
    }
    if (filt_cap == 0) {
        filtered = (seq_t *)malloc(16 * sizeof(seq_t));
        filt_cap = 16;
    }

    while (filt_len < (size_t)n) {
        uint64_t cand = next_candidate;
        if (is_prime_candidate(cand)) {
            /* store in all primes list */
            grow_seq(&all_primes, &all_len, &all_cap);
            all_primes[all_len++] = (seq_t)cand;

            /* filter by residues modulo 7 */
            uint64_t r = cand % 7ULL;
            if (r == 0ULL || r == 1ULL || r == 3ULL || r == 4ULL || r == 6ULL) {
                grow_seq(&filtered, &filt_len, &filt_cap);
                filtered[filt_len++] = (seq_t)cand;
            }
        }

        /* advance candidate */
        if (cand == 2) next_candidate = 3;
        else            next_candidate += 2;   /* skip even numbers */
    }
}

seq_t a(idx_t n)
{
    ensure_filtered(n);
    return filtered[(size_t)n - 1];
}
