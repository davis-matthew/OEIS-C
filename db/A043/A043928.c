/*
 * A043928  Numbers k such that 3 and 6 occur juxtaposed in the base-7
 *          representation of k but not of k+1.
 * https://oeis.org/A043928
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 45, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Validated: partial, pass, 45 terms through n = 45 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function below computes OEIS A043928 (Numbers k such that 3
   and 6 occur juxtaposed in the base‑7 representation of k but not of
   k+1).  It iterates over integers, tests the base‑7 property and
   caches found numbers.  The cache allows arbitrary‑order calls. */

#include <stdbool.h>
#include <stdlib.h>
#include "../utils/mem.h"

/* Return true iff the base‑7 representation of k contains the
   adjacent digits 3 and 6 (in any order).  Works for both native
   unsigned types and the arbitrary‑precision seq_t class. */
static bool has_adjacent_in_base7(seq_t k)
{
    seq_t prev = 0;
    bool has_prev = false;          /* indicates that prev holds a digit */
    while (k > 0)
    {
        seq_t d = k % 7;
        if (has_prev)
        {
            if ((prev == 3 && d == 6) || (prev == 6 && d == 3))
                return true;
        }
        prev = d;
        has_prev = true;
        k /= 7;
    }
    return false;
}

/* Cache of discovered terms.  cache[i] holds a(i+1). */
static seq_t *cache = NULL;
static idx_t cache_size   = 0;   /* number of elements stored */
static idx_t cache_capacity = 0; /* allocated capacity */
static seq_t last_processed = 0; /* largest k examined so far */

/* Ensure the cache can hold at least n terms.  Reallocate if needed. */
static void ensure_cache_capacity(idx_t n)
{
    if (cache_capacity >= n) return;
    size_t new_cap = cache_capacity ? cache_capacity * 2 : 8;
    while (new_cap < n) new_cap <<= 1;
    seq_t *tmp = mem_resize(seq_t, cache, new_cap);
    if (!tmp) abort();            /* allocation failure */
    cache = tmp;
    cache_capacity = (idx_t)new_cap;
}

/* Main sequence generator. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* offset starts at 1 */
    /* Generate terms until the nth is available. */
    while (cache_size < n)
    {
        seq_t k = ++last_processed;
        if (has_adjacent_in_base7(k) && !has_adjacent_in_base7(k + 1))
        {
            ensure_cache_capacity(cache_size + 1);
            cache[cache_size] = k;
            ++cache_size;
        }
    }
    return cache[n - 1];
}
