/*
 * A082996  a(n) = card{ x <= n : bigomega(x) = 4 }.
 * https://oeis.org/A082996
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = card{ x <= n : bigomega(x) = 4 }
 * Range:     u8 n <= 1659, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   bigomega
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/bigomega.h"

/* static cache of cumulative counts up to the largest n seen so far */
static seq_t *cum = NULL;
static size_t cap = 0;          /* allocated size (>= max n + 1) */
static idx_t max_cached = 0;    /* highest n for which cum[n] is valid */

/* Ensure the cache can hold values up to at least n.  If needed, resize
   the array.  New elements will be filled when processing new indices. */
static void ensure_cache(idx_t n)
{
    if ((idx_t)cap > n) return;          /* already large enough */

    /* resize to (n + 1) */
    size_t new_cap = (size_t)n + 1;
    seq_t *tmp = (seq_t *)realloc(cum, new_cap * sizeof(seq_t));
    if (!tmp) exit(1);     /* allocation failed; terminate */
    cum = tmp;
    cap = new_cap;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* offset is 1, but return 0 for n=0 */

    /* grow cache if necessary */
    ensure_cache(n);

    /* compute missing entries up to n */
    if (max_cached < n) {
        if (max_cached == 0)
            cum[0] = (seq_t)0;
        for (idx_t i = max_cached + 1; i <= n; ++i) {
            /* cumulative count */
            seq_t prev = cum[i - 1];
            seq_t add  = (bigomega(i) == (seq_t)4) ? (seq_t)1 : (seq_t)0;
            cum[i] = prev + add;
        }
        max_cached = n;
    }

    return cum[n];
}
