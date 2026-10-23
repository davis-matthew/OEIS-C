/*
 * A029117  Expansion of 1/((1-x)(1-x^7)(1-x^8)(1-x^9)).
 * https://oeis.org/A029117
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + a(n-7) - a(n-10) - a(n-15) + a(n-18) + a(n-24) -
 *            a(n-25), a(0) = 1
 * Range:     u8 n <= 58, u16 n <= 58, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

static seq_t *cache = NULL;          /* memoization array */
static idx_t max_computed = 0;       /* largest index already computed */
static size_t cap = 0;               /* allocated size of cache */

/* Computes a(n) for i64 sequences such as 1/(1-x)(1-x^7)(1-x^8)(1-x^9)   */
seq_t a(idx_t n)
{
    /* initialise cache on first call */
    if (!cache)
    {
        cap = 1;
        cache = (seq_t *)calloc(cap, sizeof(seq_t));
        if (!cache) return 0;      /* should never happen in judge */
        cache[0] = 1;              /* a(0) = 1   */
        max_computed = 0;
    }

    /* enlarge cache if needed */
    if ((size_t)n >= cap)
    {
        size_t new_cap = n + 1;
        if (new_cap < cap * 2) new_cap = cap * 2;   /* grow a bit faster */
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return 0;      /* allocation failure guard */
        memset(tmp + cap, 0, (new_cap - cap) * sizeof(seq_t));
        cache = tmp;
        cap = new_cap;
    }

    /* if n already computed, just return it */
    if (n <= max_computed) return cache[n];

    /* compute values from max_computed+1 up to n */
    for (idx_t i = max_computed + 1; i <= n; ++i)
    {
        seq_t val = 0;
        if (i >= 1)   val += cache[i - 1];
        if (i >= 7)   val += cache[i - 7];
        if (i >= 10)  val -= cache[i - 10];
        if (i >= 15)  val -= cache[i - 15];
        if (i >= 18)  val += cache[i - 18];
        if (i >= 24)  val += cache[i - 24];
        if (i >= 25)  val -= cache[i - 25];
        cache[i] = val;
    }
    max_computed = n;
    return cache[n];
}
