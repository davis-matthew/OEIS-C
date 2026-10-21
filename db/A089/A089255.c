/*
 * A089255  Odd numbers n such that 2*n-5 is a prime.
 * https://oeis.org/A089255
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 44, u16 n <= 57, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Sequence: A089255 – odd numbers n such that 2*n – 5 is prime.
 * This function returns the n‑th such odd integer (indexing starts at 1).
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>

#include "../utils/is_prime.h"   /* provides bool is_prime(idx_t n); */

/* --------------------------------------------------------------------- */
/*  Cache for already computed terms                                      */
/* --------------------------------------------------------------------- */
static seq_t *cache      = NULL;  /* array of terms */
static size_t cache_len  = 0;     /* number of terms stored */
static size_t cache_cap  = 0;     /* allocated capacity */

/*---------------------------------------------------------------------*/
/*  Current odd integer to test.  The first candidate is 5.            */
/*---------------------------------------------------------------------*/
static idx_t candidate = 5;

/*---------------------------------------------------------------------*/
/*  Ensure that the cache holds at least 'need' terms.                 */
/*---------------------------------------------------------------------*/
static void
ensure_cache(idx_t need)
{
    /* nothing to do if already sufficient */
    if (cache_len >= (size_t)need)
        return;

    /* initial allocation */
    if (cache == NULL) {
        cache_cap = 1024;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
    }

    /* grow array if necessary */
    while (cache_len < (size_t)need) {
        if (cache_len >= cache_cap) {
            size_t new_cap = cache_cap * 2;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) { /* allocation failed – abort */
                abort();
            }
            cache      = tmp;
            cache_cap  = new_cap;
        }

        /* test whether 2*candidate – 5 is prime */
        idx_t p = (candidate << 1) - 5;   /* 2*candidate – 5 */
        if (is_prime(p))
            cache[cache_len++] = (seq_t)candidate;

        candidate += 2;   /* next odd integer */
    }
}

/*---------------------------------------------------------------------*/
/*  Public function: nth term of the sequence.                         */
/*---------------------------------------------------------------------*/
seq_t
a(idx_t n)
{
    if (n == 0)
        return 0;           /* undefined for n == 0, return 0 safely */

    ensure_cache(n);
    return cache[(size_t)(n - 1)];
}
