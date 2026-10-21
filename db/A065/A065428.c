/*
 * A065428  Numbers k such that no x^2 mod k is prime.
 * https://oeis.org/A065428
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice, hard, more
 * Formula:   not stated
 * Range:     u8 n <= 22, u16 n <= 30, u32 n <= 30, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   isPrime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A065428: numbers k such that no x^2 (mod k) is prime.
 *  The function generates the sequence by testing successive integers
 *  until it has produced the requested index n.  A cached array of
 *  already found terms is maintained so that subsequent calls are
 *  fast in any order.
 *
 *  For each candidate k, the set of quadratic residues modulo k is
 *  obtained by iterating x = 0 .. ⌊k/2⌋ and using the recurrence
 *
 *      r_{x+1} = (r_x + 2x + 1) mod k
 *
 *  If any residue r ≥ 2 is prime then k is rejected; otherwise k is
 *  accepted and stored in the cache.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/isPrime.h"

/* Forward declaration of helper that tests a single k          */
static inline bool is_good_k(idx_t k);

/* Cached sequence storage                                 */
static seq_t *cache      = NULL;   /* array of terms */
static size_t cache_len  = 0;      /* number of terms stored */
static size_t cache_cap  = 0;      /* capacity of cache */
static idx_t  next_k     = 1;     /* next integer to test */

/* Expand the cache when needed                               */
static void ensure_cache_capacity(void)
{
    if (cache_len >= cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 256;
        seq_t *p = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!p) abort();                 /* out of memory */
        cache = p;
        cache_cap = new_cap;
    }
}

/* Return true if k has no prime quadratic residue modulo k   */
static inline bool is_good_k(idx_t k)
{
    if (k == 0) return false;            /* not used, but safe */

    seq_t r = 0;                     /* residue for x = 0 */
    seq_t modk = (seq_t)k;

    /* iterate x = 1 .. floor(k/2) ; residues repeat for larger x   */
    for (idx_t x = 1; x <= (k >> 1); ++x) {
        /* r = (r + (2*x - 1)) mod k  */
        unsigned __int128 tmp = (unsigned __int128)r + (unsigned __int128)(2 * x - 1);
        r = (seq_t)(tmp % (unsigned __int128)modk);

        if (r >= 2 && isPrime((idx_t)r))
            return false;                  /* prime residue found */
    }
    return true;                            /* all residues composite (or 0,1) */
}

/* Public function required by the test harness                 */
seq_t a(idx_t n)
/* Return the n-th term of A065428 (offset 1).  The sequence
 *  is built incrementally and memoised across calls. */
{
    if (n == 0) return 0;                 /* offset starts at 1 */

    /* Build sequence until we have at least n terms */
    while (cache_len < n) {
        if (is_good_k(next_k)) {           /* accept the candidate   */
            ensure_cache_capacity();
            cache[cache_len++] = (seq_t)next_k;
        }
        next_k++;                          /* try next integer       */
    }
    return cache[n - 1];
}
