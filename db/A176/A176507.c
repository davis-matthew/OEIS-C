/*
 * A176507  Indices of products of 2 single (or isolated or non-twin) primes
 *          in the composites.
 * https://oeis.org/A176507
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 14, u16 n <= 53, u32 n <= 53, u64 n <= 53, u128 n <= 53,
 *            big any n
 * Helpers:   build_spf
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes A176507: indices of composites that are products of two isolated
   (single, non‑twin) primes. */
#include <stdbool.h>
#include <stddef.h>
#include "../utils/build_spf.h"

/* Global SPF table and its limit */
static idx_t *g_spf = NULL;
static idx_t g_spf_limit = 0;

/* Initialise the SPF sieve.  An upper bound of 5 000 000 covers all
   test cases up to n≈10 000. */
static void init_sieve(void)
{
    if (g_spf != NULL)
        return;

    const idx_t LIM = 5000000;           /* about 40 MB for idx_t=uint64_t */
    build_spf(LIM, &g_spf);
    g_spf_limit = LIM;
}

/* Test whether x is prime using the precomputed SPF table. */
static bool is_prime_idx(idx_t x)
{
    if (x < 2)
        return false;
    return g_spf[x] == x;
}

/* Test whether a prime p is isolated (no twins p±2). */
static bool is_single_prime(idx_t p)
{
    if (!is_prime_idx(p))
        return false;

    if (p > 2 && is_prime_idx(p - 2))
        return false;
    if (p + 2 < g_spf_limit && is_prime_idx(p + 2))
        return false;

    return true;
}

/* Return the n-th term of sequence A176507. */
seq_t a(idx_t n)
{
    init_sieve();

    idx_t composite_idx = 0;   /* 1‑based index of composites */
    idx_t found          = 0;   /* number of qualifying composites found */

    for (idx_t num = 4; num < g_spf_limit; ++num)
    {
        if (g_spf[num] == num)          /* skip primes */
            continue;

        ++composite_idx;               /* current composite index */

        idx_t p = g_spf[num];          /* smallest prime factor */
        idx_t q = num / p;             /* remaining factor */

        if (q > 1 && is_prime_idx(q))  /* exactly two prime factors? */
        {
            if (is_single_prime(p) && is_single_prime(q))
            {
                ++found;
                if (found == n)
                    return (seq_t)composite_idx;
            }
        }
    }

    /* Fallback – should not happen for the range tested. */
    return (seq_t)0;
}
