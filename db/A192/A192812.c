/*
 * A192812  Constant term in the reduction of the polynomial x^(2*n) + x^n + 1
 *          by x^3 -> x + 1. See Comments.
 * https://oeis.org/A192812
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 12, u16 n <= 22, u32 n <= 42, u64 n <= 81, u128 n <= 160,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: OEIS A192812
   Constant term in the reduction of x^(2*n)+x^n+1 by x^3 → x+1.
   Recurrence: a(n)=3*a(n-1)-2*a(n-2)-a(n-4)+a(n-5)-a(n-6)+a(n-7),
   with a(0)=3,a(1)=1,a(2)=1,a(3)=3,a(4)=3,a(5)=5,a(6)=7. */
#include <stddef.h>
#include <stdlib.h>

/* Memoised cache for previously computed terms.
   cache_len holds the number of populated entries. */
static seq_t *cache = NULL;
static size_t cache_len = 0;

/* Ensure that cache contains values 0 … n (inclusive).  Allocate
   or grow the cache as necessary and fill in any new terms with the
   recurrence.  All arithmetic is performed in seq_t. */
static void ensure_cache(idx_t n)
{
    size_t need = (size_t)n + 1;          /* number of elements required */

    if (need <= cache_len) return;          /* already available */

    /* Allocate a new cache big enough for the needed entries. */
    seq_t *new_cache = (seq_t *)malloc(need * sizeof(seq_t));
    if (!new_cache) abort();                /* fatal allocation error */

    /* Copy any previously stored values. */
    for (size_t i = 0; i < cache_len; ++i) new_cache[i] = cache[i];

    /* Initialise and compute new entries.  i records the current index
       being written into the new cache. */
    for (size_t i = cache_len; i < need; ++i)
    {
        if (i == 0)      new_cache[i] = (seq_t)3;
        else if (i == 1 || i == 2) new_cache[i] = (seq_t)1;
        else if (i == 3 || i == 4) new_cache[i] = (seq_t)3;
        else if (i == 5)        new_cache[i] = (seq_t)5;
        else if (i == 6)        new_cache[i] = (seq_t)7;
        else /* i >= 7 */ {
            new_cache[i] = ((seq_t)3 * new_cache[i-1])
                        - ((seq_t)2 * new_cache[i-2])
                        - new_cache[i-4]
                        + new_cache[i-5]
                        - new_cache[i-6]
                        + new_cache[i-7];
        }
    }

    /* Replace the old cache with the new one. */
    free(cache);
    cache = new_cache;
    cache_len = need;
}

/* Public function for the sequence. */
seq_t a(idx_t n)
{
    ensure_cache(n);
    return cache[(size_t)n];
}
