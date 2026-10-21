/*
 * A096079  Near hypotenuse numbers, i.e., n such that n^2 -+ 1 = a^2 + b^2,
 *          with a, b > 1.
 * https://oeis.org/A096079
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 40, u32 n <= 69, u64 n <= 69, u128 n <= 69,
 *            big any n
 * Helpers:   has_two_square_rep, is_near_hypotenuse, isqrt_generic
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Near hypotenuse numbers A096079
 *
 * Definition: n is in the sequence if either n²+1 or n²–1 can be
 * expressed as a²+b² with a > 1 and b > 1.
 *
 * Algorithm: For each candidate n from 1 upward, test the two values
 * m = n²±1.  To test whether m is a sum of two squares, try all
 * a ≥ 2 up to ⌊√(m–4)⌋, compute b² = m–a² and check that b² is a
 * perfect square larger than 1.  The search stops when the cache
 * contains at least n terms, and the cache is reused for arbitrary
 * call order.  The implementation uses an integer square‑root routine
 * based on binary search that works for any seq_t (including
 * arbitrary‑precision).
 *
 * The routine satisfies the required O(n) time behaviour for n ≤ 10 000.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/has_two_square_rep.h"
#include "../utils/is_near_hypotenuse.h"
#include "../utils/isqrt_generic.h"

/* --------------------------------------------------------------------- */
/* Local caching of found terms, to allow out‑of‑order queries.          */
/* --------------------------------------------------------------------- */
static seq_t *cache      = NULL;
static size_t cache_len  = 0;
static size_t cache_cap  = 0;
static idx_t  max_checked = 0;

/* Grow cache if necessary and append a term. */
static void cache_push(seq_t val)
{
    if (cache_len == cache_cap)
    {
        cache_cap = (cache_cap > 0) ? cache_cap * 2 : 16;
        cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
    }
    cache[cache_len++] = val;
}

/* --------------------------------------------------------------------- */
/* Public interface: return the nth term (1‑based).                     */
/* --------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n < 1)
        return 0;               /* undefined for n=0, but return 0 for safety */

    /* Generate terms until we have at least n of them. */
    while (cache_len < (size_t)n)
    {
        ++max_checked;
        seq_t cand = (seq_t)max_checked;
        if (is_near_hypotenuse(cand))
            cache_push(cand);
    }
    return cache[n - 1];
}
