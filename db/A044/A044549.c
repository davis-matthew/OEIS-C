/*
 * A044549  Numbers n such that string 4,2 occurs in the base 7 representation
 *          of n but not of n+1.
 * https://oeis.org/A044549
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 42, u32 n <= 42, u64 n <= 42, u128 n <= 42,
 *            big any n
 * Validated: partial, pass, 42 terms through n = 42 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A044549: numbers n such that the base‑7 representation of n
   contains the digit pair 4,2 but the base‑7 representation of n+1
   does NOT.  The function iteratively generates terms on the fly
   and stores them in a memoised cache.  It works for any seq_t and idx_t
   types supplied by the build. */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/mem.h"

/* Return true if x (written in base 7) contains the substring "42". */
static bool has_42(seq_t x)
{
    if (x < (seq_t)7) return false;            /* one digit cannot contain it */

    seq_t last = x % (seq_t)7;                 /* least‑significant digit */
    seq_t rest = x / (seq_t)7;

    while (rest)
    {
        seq_t cur = rest % (seq_t)7;
        if (last == (seq_t)2 && cur == (seq_t)4) return true;
        last = cur;
        rest /= (seq_t)7;
    }
    return false;
}

/* Memoisation state */
static seq_t  *cache      = NULL;   /* array of found terms              */
static size_t cache_cap  = 0;       /* allocated capacity in elements    */
static idx_t  cache_size = 0;       /* number of terms stored            */
static seq_t  next_test  = (seq_t)1;/* smallest integer not yet tested   */

/* Oracle for OEIS A044549 */
seq_t a(idx_t n)
{
    if (n == (idx_t)0) return (seq_t)0;  /* offset is 1 */

    /* Keep generating until we have at least n terms. */
    while (cache_size < n)
    {
        /* Test current integer.  If it satisfies the property, cache it. */
        if (has_42(next_test) && !has_42(next_test + (seq_t)1))
        {
            /* Grow the cache if needed. */
            if (cache_size == cache_cap)
            {
                size_t new_cap = cache_cap ? cache_cap * 2 : 16;
                seq_t *tmp = mem_resize(seq_t, cache, new_cap);
                if (!tmp) abort();          /* allocation failure */
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_size++] = next_test;
        }
        ++next_test;                     /* move to the next integer to test */
    }

    return cache[n - 1];
}
