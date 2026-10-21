/*
 * A206754  Number of (n+1) X 2 0..2 arrays with every 2 X 3 or 3 X 2 subblock
 *          having no more than three equal edges, and new values 0..2
 *          introduced in row major order.
 * https://oeis.org/A206754
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 20, u128 n <= 42,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS entry: A206754
   Recurrence (empirical): 
   a(n) = 7*a(n-1) + 5*a(n-2) + 34*a(n-3) - 2*a(n-4) + 12*a(n-5)   for n > 6
   Initial values:
   a(1) = 14
   a(2) = 106
   a(3) = 861
   a(4) = 7010
   a(5) = 56970
   a(6) = 463064
*/

#include <stdlib.h>
#include <string.h>

/* Global cache for computed terms */
static seq_t *cache = NULL;           /* pointer to array of seq_t, index from 0..n */
static size_t cache_cap = 0;          /* allocated capacity */
static idx_t  max_computed = 0;       /* highest index computed and stored */

/* Utility: ensure cache has capacity at least 'needed' elements */
static void ensure_cache_size(size_t needed)
{
    if (needed <= cache_cap)
        return;
    size_t new_cap = cache_cap > 0 ? cache_cap : 16;
    while (new_cap < needed)
        new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp)
        exit(EXIT_FAILURE); /* allocation failure */
    /* zero the new part of the cache */
    if (new_cap > cache_cap)
        memset(tmp + cache_cap, 0, (new_cap - cache_cap) * sizeof(seq_t));
    cache = tmp;
    cache_cap = new_cap;
}

/* Main function: a(n) */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;   /* undefined for OEIS offset, return 0 */

    /* Ensure the cache can hold up to index n */
    ensure_cache_size((size_t)n + 1);

    /* Compute terms up to n if necessary */
    if (n > max_computed) {
        for (idx_t i = max_computed + 1; i <= n; ++i) {
            if (i == 1)           cache[1] = (seq_t)14;
            else if (i == 2)      cache[2] = (seq_t)106;
            else if (i == 3)      cache[3] = (seq_t)861;
            else if (i == 4)      cache[4] = (seq_t)7010;
            else if (i == 5)      cache[5] = (seq_t)56970;
            else if (i == 6)      cache[6] = (seq_t)463064;
            else {
                seq_t term = 0;
                term += cache[i - 1] * (seq_t)7;
                term += cache[i - 2] * (seq_t)5;
                term += cache[i - 3] * (seq_t)34;
                term += cache[i - 5] * (seq_t)12;
                term -= cache[i - 4] * (seq_t)2;
                cache[i] = term;
            }
        }
        max_computed = n;
    }

    return cache[n];
}
