/*
 * A091628  Concatenation of n 2's followed by 3.
 * https://oeis.org/A091628
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = 10*a(n-1) - 7, a(1) = 23
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 18, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* (Equivalently: a(n) is the integer formed by n copies of the digit 2 followed by a 3) */

#include <stdlib.h>          /* for malloc, realloc, exit, EXIT_FAILURE */

static seq_t *cache   = NULL;   /* 1‑based cache of computed terms */
static idx_t  size   = 0;       /* number of terms currently stored (at least 1 if size>0) */
static idx_t  capacity = 0;     /* allocated length of the cache array */

/* Ensure the cache can store at least <needed> elements (including index 0).  */
static void ensure_capacity(idx_t needed)
{
    if (needed <= capacity) return;

    idx_t new_cap = capacity ? capacity << 1 : 4;
    while (new_cap < needed) new_cap <<= 1;

    seq_t *tmp = (seq_t *)realloc(cache, (size_t)new_cap * sizeof(seq_t));
    if (!tmp) { abort(); }
    cache     = tmp;
    capacity  = new_cap;
}

/* Compute the nth term of A091628.  */
seq_t a(idx_t n)
{
    if (n == 0) return 0;      /* offset is 1; n=0 is outside the sequence */

    /* Initialise cache on the first call. */
    if (!cache)
    {
        ensure_capacity(1);    /* at least index 0 */
        cache[0] = 0;          /* dummy placeholder */
    }

    /* Compute terms up to n if necessary. */
    if (size < n)
    {
        ensure_capacity(n + 1);   /* include index 0 */

        for (idx_t i = size + 1; i <= n; ++i)
        {
            seq_t val;
            if (i == 1)
                val = (seq_t)23;                                   /* a(1) */
            else
                val = cache[i - 1] * (seq_t)10 - (seq_t)7;          /* a(i) = 10*a(i-1) – 7 */

            cache[i] = val;
        }

        size = n;
    }

    return cache[n];
}
