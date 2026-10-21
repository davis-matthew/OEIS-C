/*
 * A054946  Number of strongly connected labeled tournaments on n nodes.
 * https://oeis.org/A054946
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 8, u64 n <= 11, u128 n <= 16,
 *            big any n
 * Helpers:   binomial, pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A054946: Number of strongly‑connected labeled tournaments.
   Recurrence: Let F(n)=2^(n*(n-1)/2).
   a(1)=1,  F(n)=a(n)+∑_{s=1}^{n-1} binomial(n,s)*a(s)*F(n-s).
   The function memoises values so that calls can come in any order. */
#include <stdlib.h>
#include <stdbool.h>

/* Helper routines supplied by the build */
#include "../utils/binomial.h"
#include "../utils/pow_uint.h"

/*----------------- Memoisation storage -----------------*/
static seq_t *a_cache = NULL;   /* indexed 1.. */
static seq_t *f_cache = NULL;   /* indexed 1.. */
static size_t  a_cap   = 0;     /* allocated size of a_cache   */
static size_t  f_cap   = 0;     /* allocated size of f_cache   */
static idx_t   a_size  = 0;     /* highest n with a[n] computed */
static idx_t   f_size  = 0;     /* highest n with F[n] computed */

/* Grow a cache to permit indices up to 'n' (0‑based). */
static void grow_cache(idx_t n, seq_t **cache, size_t *cap)
{
    if ((size_t)n + 1 > *cap) {
        size_t new_cap = (*cap) ? (*cap) * 2 : 16;
        while (new_cap <= (size_t)n + 1) new_cap *= 2;
        *cache = (seq_t*)realloc(*cache, new_cap * sizeof(seq_t));
        *cap   = new_cap;
    }
}

/* Main sequence entry point.  No memoisation is required for n<1. */
seq_t a(idx_t n)
{
    if (n < 1) return 0;            /* undefined for n=0, but safe */
    /* Ensure caches can hold up to index n. */
    grow_cache(n, &a_cache, &a_cap);
    grow_cache(n, &f_cache, &f_cap);

    /* Initialise base cases if not already done. */
    if (a_size < 1) a_cache[1] = (seq_t)1;
    if (a_size < 2) a_cache[2] = (seq_t)0;

    /* Pre‑compute F(i)=2^(i*(i-1)/2) for any new i.  Use pow_uint to
       handle large exponents without overflow in intermediate types. */
    for (idx_t i = f_size + 1; i <= n; ++i) {
        idx_t exp = (idx_t)i * ((idx_t)i - 1u) / 2u;   /* fits in idx_t */
        f_cache[i] = pow_uint(2ull, exp);
    }
    f_size = n;

    /* Compute a(i) for every i that hasn't been computed yet. */
    for (idx_t i = a_size + 1; i <= n; ++i) {
        seq_t Fi   = f_cache[i];
        seq_t sum  = 0;
        for (idx_t s = 1; s < i; ++s) {
            seq_t term = binomial(i, s) * a_cache[s] * f_cache[i - s];
            sum += term;
        }
        a_cache[i] = Fi - sum;    /* subtraction stays in seq_t type */
    }
    a_size = n;

    return a_cache[n];
}
