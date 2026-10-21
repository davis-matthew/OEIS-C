/*
 * A050758  Primes containing at least one pair of consecutive equal digits.
 * https://oeis.org/A050758
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the n‑th prime p that contains at least one pair of
 *            consecutive equal digits
 * Range:     u8 n <= 8, u16 n <= 1667, u32 n <= 1668, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime_big, no_consecutive_equal_digits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function generates primes in increasing order, keeps only those with
   repeated adjacent digits, and returns the requested term. */

/* External utilities provided by the build environment */
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime_big.h"
#include "../utils/no_consecutive_equal_digits.h"

static seq_t *g_terms      = NULL;  /* stored primes with consecutive equal digits */
static size_t g_terms_len  = 0;     /* number of stored terms */
static size_t g_terms_cap  = 0;     /* allocated capacity */
static seq_t  g_candidate  = 0;     /* next odd prime candidate to test */
static bool   g_initialized = false;

/* Ensure that at least n terms are available */
static void ensure_terms(idx_t n)
{
    if (!g_initialized) {
        g_terms_cap   = 16;
        g_terms       = (seq_t *)malloc(g_terms_cap * sizeof(seq_t));
        g_candidate   = (seq_t)11;          /* start with first odd prime > 10 */
        g_initialized = true;
    }

    if (!g_terms) return;

    while (g_terms_len < (size_t)n) {
        /* Test current candidate */
        if (is_prime_big(g_candidate) && !no_consecutive_equal_digits(g_candidate)) {
            /* Grow the array if necessary */
            if (g_terms_len >= g_terms_cap) {
                g_terms_cap <<= 1;
                g_terms = (seq_t *)realloc(g_terms, g_terms_cap * sizeof(seq_t));
            }
            g_terms[g_terms_len++] = g_candidate;
        }

        /* Advance to the next odd candidate */
        g_candidate += (seq_t)2;

        /* Avoid infinite loop when seq_t overflows (e.g., 8‑bit builds) */
        if (g_candidate < (seq_t)3) break;
    }
}

/* Return the n‑th term (n ≥ 1). */
seq_t a(idx_t n)
{
    if (n < (idx_t)1) return (seq_t)0;     /* undefined for n = 0 */
    ensure_terms(n);
    return g_terms[(size_t)(n - 1)];        /* zero‑based indexing inside array */
}
