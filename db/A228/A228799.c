/*
 * A228799  Number of 4 X n binary arrays with top left element equal to 1 and
 *          no two ones adjacent horizontally or nw-se.
 * https://oeis.org/A228799
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + 20*a(n-2) + 27*a(n-3) - 14*a(n-4) - 25*a(n-5) +
 *            4*a(n-6) + 5*a(n-7) - a(n-8) (n ≥ 9) Base values: a(1)=8,
 *            a(2)=13, a(3)=156, a(4)=586, a(5)=3957, a(6)=19474, a(7)=111966,
 *            a(8)=596273
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 13, u64 n <= 26, u128 n <= 52,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

/* Product and accumulation use seq_t; the constants are cast to seq_t
   so that multiplication occurs in seq_t arithmetic. */

static const seq_t init_vals[9] = {
    0,          /* unused index 0 */
    8,          /* a(1) */
    13,         /* a(2) */
    156,        /* a(3) */
    586,        /* a(4) */
    3957,       /* a(5) */
    19474,      /* a(6) */
    111966,     /* a(7) */
    596273      /* a(8) */
};

/* Cache of previously computed values.  cache[0] is unused. */
static seq_t *cache = NULL;
static size_t cap = 0;

/* Ensure cache has room for indices 0..n inclusive.  New entries are
   zero-initialised to signal “not yet computed”. */
static void ensure_cache(idx_t n)
{
    if (n < cap)
        return;

    size_t new_cap = cap ? cap * 2 : 1;
    while (n >= new_cap)
        new_cap *= 2;

    seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!new_cache)
        exit(1);               /* out of memory */
    /* initialise new entries to 0 */
    for (size_t i = cap; i < new_cap; ++i)
        new_cache[i] = 0;
    cache = new_cache;
    cap = new_cap;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;               /* undefined for n=0, return 0 */

    ensure_cache(n);

    for (idx_t i = 1; i <= n; ++i)
    {
        if (cache[i])
            continue;           /* already computed */

        if (i <= 8)
        {
            cache[i] = init_vals[i];
        }
        else
        {
            seq_t val = cache[i - 1];
            val += (seq_t)20 * cache[i - 2];
            val += (seq_t)27 * cache[i - 3];
            val -= (seq_t)14 * cache[i - 4];
            val -= (seq_t)25 * cache[i - 5];
            val += (seq_t)4  * cache[i - 6];
            val += (seq_t)5  * cache[i - 7];
            val -= cache[i - 8];
            cache[i] = val;
        }
    }

    return cache[n];
}
