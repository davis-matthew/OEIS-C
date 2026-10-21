/*
 * A075761  Numbers n such that there are at least 3 integers k from the set
 *          {2,3,4,5,6,7,8,9} such that the digital sum of each base k
 *          representation of n equals k.
 * https://oeis.org/A075761
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 23, u16 n <= 51, u32 n <= 51, u64 n <= 51, u128 n <= 51,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) is the n-th integer that has at least three bases k∈{2,…,9}
 * such that the digital sum of that integer written in base k equals k.
 * The function uses a simple on‑demand sieve and caches all found terms.
 */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>

/*--- Static cache for the sequence -------------------------------------*/
static seq_t *cache     = NULL;   /* array of found terms          */
static size_t cache_cap = 0;      /* allocated capacity            */
static size_t cache_len = 0;      /* number of terms stored        */
static idx_t  last_checked = 1;   /* next integer to test          */

/*--- Test whether a number satisfies the definition --------------------*/
static bool ok_candidate(seq_t val)
{
    int good = 0;
    for (int base = 2; base <= 9 && good < 3; ++base) {
        seq_t x  = val;
        seq_t sum = 0;
        while (x) {
            sum += x % (seq_t)base;
            x   /= (seq_t)base;
        }
        if (sum == (seq_t)base)
            ++good;
    }
    return good >= 3;
}

/*--- Ensure the cache is large enough ----------------------------------*/
static void grow_cache(size_t new_cap)
{
    if (new_cap <= cache_cap)
        return;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    cache = tmp;
    cache_cap = new_cap;
}

/*--- Main sequence function ---------------------------------------------*/
seq_t a(idx_t n)
{
    /* Generate terms until we have at least n of them. */
    while (cache_len < n) {
        /* Look for the next valid integer. */
        while (1) {
            seq_t cand = (seq_t)last_checked;
            ++last_checked;
            if (ok_candidate(cand)) {
                /* Append to cache */
                if (cache_len == cache_cap)
                    grow_cache(cache_cap ? cache_cap * 2 : 4);
                cache[cache_len++] = cand;
                break;
            }
        }
    }
    return cache[n - 1];
}
