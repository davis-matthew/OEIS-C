/*
 * A133635  Nonprime numbers k such that binomial(k+p,k) mod k = 1, where p=5.
 * https://oeis.org/A133635
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 32, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   alloc_seq, free_seq, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A133635 – Nonprime k with C(k+5,k) ≡ 1 (mod k)  (p = 5) */
/* The function returns the n-th term (1‑based) of that sequence. */

#include <stdlib.h>
#include <stdbool.h>

/* helpers from the utils library */
#include "../utils/is_prime.h"
#include "../utils/alloc_seq.h"
#include "../utils/free_seq.h"

/* --- cache of found terms ---------------------------------------------- */
static seq_t *cache = NULL;       /* dynamic array of terms */
static size_t cache_size = 0;     /* number of terms stored */
static size_t cache_cap   = 0;     /* current capacity of cache */
static idx_t  last_checked = 1;   /* last k that was examined */

/* compute (k+5 choose 5) and test the congruence modulo k */
static bool good_candidate(idx_t k)
{
    /* compute binomial(k+5,5) exactly in 128‑bit arithmetic */
    unsigned __int128 res = 1;
    for (idx_t i = 1; i <= 5; ++i)
        res = (res * (k + i)) / i;   /* exact division   */

    /* check the congruence */
    return ((res % k) == 1u);
}

seq_t a(idx_t n)
{
    if (n < 1)
        return 0;   /* undefined, but harmless */

    /* expand the cache until it contains n terms */
    while (cache_size < n)
    {
        /* find the next non‑prime candidate  */
        do {
            ++last_checked;
        } while (is_prime(last_checked));

        /* test the congruence */
        if (good_candidate(last_checked))
        {
            /* enlarge storage if necessary */
            if (cache_size >= cache_cap)
            {
                size_t new_cap = (cache_cap == 0 ? 4 : cache_cap * 2);
                seq_t *new_ptr = alloc_seq(new_cap);
                if (!new_ptr) abort();                /* allocation failure */
                for (size_t i = 0; i < cache_size; ++i)
                    new_ptr[i] = cache[i];
                if (cache)
                    free_seq(cache);                  /* free old array */
                cache = new_ptr;
                cache_cap = new_cap;
            }
            /* store the new term */
            cache[cache_size++] = (seq_t)last_checked;
        }
    }
    return cache[n - 1];
}
