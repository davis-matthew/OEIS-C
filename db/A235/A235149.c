/*
 * A235149  Primes p such that (p reversed) + 7 is also a prime.
 * https://oeis.org/A235149
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 97, u32 n <= 97, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   alloc_seq_array, free_seq_array, is_prime, is_prime_big, rev
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A235149: Primes p such that reverse(p) + 7 is also prime
 * Computation method: incremental search with memoised cache.
 * The first term a(1)=43, a(2)=61, … as specified in the OEIS entry. */

#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "../utils/is_prime.h"      /* bool is_prime(idx_t x); */
#include "../utils/rev.h"          /* seq_t rev(idx_t x); */
#include "../utils/is_prime_big.h" /* bool is_prime_big(seq_t n); */
#include "../utils/alloc_seq_array.h" /* seq_t *alloc_seq_array(size_t n); */
#include "../utils/free_seq_array.h"  /* void free_seq_array(seq_t *p); */

static seq_t *cache      = NULL;   /* array of found primes */
static size_t cache_cap  = 0;      /* allocated capacity (in elements) */
static size_t cache_cnt  = 0;      /* number of terms currently stored */
static idx_t  last_check = 2;      /* the integer last examined for prime */

/* Grow the cache to hold at least 'need' elements. */
static void grow_cache(size_t need)
{
    if (need <= cache_cap)
        return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 4;
    while (new_cap < need)
        new_cap *= 2;
    seq_t *tmp = alloc_seq_array(new_cap);
    if (!tmp && new_cap > 0)
        abort();                /* allocation failure */
    if (cache && cache_cnt > 0)
    {
        memcpy(tmp, cache, cache_cnt * sizeof(seq_t));
        free_seq_array(cache);
    }
    cache     = tmp;
    cache_cap = new_cap;
}

/* Generate terms until the cache contains at least 'n' elements. */
static void fill_cache(idx_t n)
{
    while (cache_cnt < n)
    {
        ++last_check;
        if (!is_prime(last_check))
            continue;                     /* skip composites */

        seq_t rev_val = rev(last_check);
        seq_t test    = rev_val + (seq_t)7;   /* reverse(p) + 7 */
        if (is_prime_big(test))
        {
            grow_cache(cache_cnt + 1);
            cache[cache_cnt++] = (seq_t)last_check;
        }
    }
}

/* Public sequence function */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;                       /* undefined for n<1 */
    fill_cache(n);
    return cache[n - 1];
}
