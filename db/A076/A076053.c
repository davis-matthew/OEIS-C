/*
 * A076053  Rearrangement of natural numbers such that the 2n-th partial sum
 *          is composite and (2n-1)-th partial sum is prime.
 * https://oeis.org/A076053
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 21, u16 n <= 72, u32 n <= 72, u64 n <= 72, u128 n <= 72,
 *            big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Greedy construction of OEIS A076053:
   permutation of natural numbers such that the (2n–1)-st partial sum is prime
   and the 2n‑th partial sum is composite.  */
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "../utils/is_prime_big.h"

/* Static tables for caching the sequence and used numbers. */
static seq_t *cache     = NULL;      /* cache[0] = a(1) */
static size_t cache_len = 0;         /* number of terms stored */
static size_t cache_cap = 0;         /* allocated capacity of cache */

static bool *used   = NULL;          /* used[i] = true if integer i is used */
static size_t used_len = 0;          /* allocated length of used array */

static seq_t cumulative_sum = 0;     /* partial sum of the sequence */

/* Ensure that `used` has room for index `k` (1‑based).  */
static void ensure_used_capacity(idx_t k)
{
    if (used_len > (size_t)k) return;
    size_t new_len = used_len ? used_len * 2 : 8;
    while (new_len <= (size_t)k) new_len <<= 1;

    bool *tmp = (bool *)realloc(used, new_len * sizeof(bool));
    if (!tmp) abort();
    if (used_len < new_len)
        memset(tmp + used_len, 0, (new_len - used_len) * sizeof(bool));
    used = tmp;
    used_len = new_len;
}

/* Ensure that `cache` can hold at least `k` terms.  */
static void ensure_cache_capacity(idx_t k)
{
    if (cache_cap >= (size_t)k) return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 8;
    while (new_cap < (size_t)k) new_cap <<= 1;

    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) abort();
    cache = tmp;
    cache_cap = new_cap;
}

/* Return the nth term of A076053 (n ≥ 1).  */
seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* undefined for n = 0 */

    /* Build the sequence up to n.  */
    ensure_cache_capacity(n);
    while (cache_len < (size_t)n)
    {
        /* We are about to compute the (cache_len+1)-th term. */
        bool want_prime = ((cache_len + 1) & 1UL) != 0;  /* odd index → prime */

        for (idx_t x = 1; ; ++x)
        {
            ensure_used_capacity(x);
            if (used[(size_t)x]) continue;  /* already used */

            /* Test the new partial sum.  */
            seq_t new_sum = cumulative_sum + (seq_t)x;
            bool ok;
            if (want_prime)
                ok = new_sum > (seq_t)1 && is_prime_big(new_sum);
            else
                ok = new_sum > (seq_t)1 && !is_prime_big(new_sum);

            if (ok)
            {
                cache[cache_len++] = (seq_t)x;
                used[(size_t)x] = true;
                cumulative_sum = new_sum;
                break;
            }
        }
    }

    return cache[(size_t)n - 1];
}
