/*
 * A065735  Largest square <= product of first n primes.
 * https://oeis.org/A065735
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = floor( sqrt( ∏_{i=1}^{n} p_i ) )^2 (Largest perfect
 *            square ≤ product of first n primes)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 15, u128 n <= 20,
 *            big any n
 * Helpers:   floor_sqrt, seq_mul, sieve_first_n_primes
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/sieve_first_n_primes.h"
#include "../utils/seq_mul.h"
#include "../utils/floor_sqrt.h"

/* Cache for primorials: prim_cache[k] = p_1 * p_2 * ... * p_k */
static seq_t *prim_cache   = NULL;   /* allocated size: prim_cache_sz */
static size_t prim_cache_sz = 0;
static idx_t  prim_cache_n  = 0;     /* highest n computed so far */

/* Cache for the first primes */
static uint32_t *prime_cache   = NULL; /* allocated size: prime_cache_sz */
static size_t   prime_cache_sz = 0;    /* number of primes stored */

/* Ensure that prim_cache[n] and prime_cache[n-1] exist and are filled */
static void
ensure_caches(idx_t n)
{
    /* 1) Ensure we have the first n primes */
    if (prime_cache_sz < (size_t)n) {
        if (prime_cache) free(prime_cache);
        sieve_first_n_primes(n, &prime_cache, &prime_cache_sz);
    }

    /* 2) Allocate/expand prim_cache to hold n+1 entries (incl. index 0) */
    size_t needed = (size_t)n + 1;
    if (prim_cache_sz < needed) {
        seq_t *tmp = (seq_t *)realloc(prim_cache, needed * sizeof(seq_t));
        if (!tmp) exit(EXIT_FAILURE);          /* out of memory */
        prim_cache = tmp;
        prim_cache_sz = needed;
        if (prim_cache_n == 0) prim_cache[0] = (seq_t)1;
    }

    /* 3) Compute primorials up to n if not already done */
    if (prim_cache_n < n) {
        /* prim_cache[0] is already 1 */
        for (idx_t i = prim_cache_n; i < n; ++i) {
            /* i is 0‑based index into prime_cache */
            prim_cache[i + 1] = seq_mul(prim_cache[i], (seq_t)prime_cache[i]);
        }
        prim_cache_n = n;
    }
}

/* OEIS A065735: Largest square ≤ product of first n primes */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* offset is 1, guard against zero */

    ensure_caches(n);

    seq_t product = prim_cache[n];
    seq_t root    = floor_sqrt(product);   /* floor(sqrt(product)) */
    return seq_mul(root, root);            /* square of the root */
}
