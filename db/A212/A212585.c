/*
 * A212585  Walks of length n on the x-axis using steps {1,-1} and visiting no
 *          point more than twice.
 * https://oeis.org/A212585
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, walk
 * Formula:   not stated
 * Range:     u8 n <= 12, u16 n <= 29, u32 n <= 42, u64 n <= 42, u128 n <= 42,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*

  Sequence A212585: walks of length n on the x‑axis using steps {1,-1}
  and visiting no point more than twice.

  Generating function (OEIS):
     G(x) = (1 + x + 2x³ – 3x⁵ + 2x⁷ – x⁸)
            / ((1 – x²)² (1 – x – x⁴))

  From the denominator  D(x) = 1 – x – 2x² + 2x³ – x⁵ + 2x⁶ – x⁸
  we obtain the linear recurrence (valid for n ≥ 9):
      a[n] = a[n‑1] + 2·a[n‑2] – 2·a[n‑3] + a[n‑5] – 2·a[n‑6] + a[n‑8]

  The first nine terms are
      a[0]…a[8] = 1, 2, 4, 8, 12, 18, 26, 40, 56
  (a[8] is obtained from the equation at n = 8).

  The routine keeps a memoised array so that calls in any order stay
  fast.  All arithmetic is done with the user‑supplied integer type
  seq_t (unsigned, arbitrary‑precision or up to 128 bits).

*/

#include <stdlib.h>   /* malloc, realloc, free */

static seq_t *cache = NULL;       /* memoised values */
static idx_t  cap   = 0;          /* allocated capacity */
static idx_t  last  = (idx_t)-1;   /* highest index computed */

/* initialise the cache with the first nine terms of the sequence */
static void init_cache_for_a212585(void)
{
    /* the first nine terms (a[0] … a[8]) */
    static const seq_t init[9] = {
        (seq_t)1, (seq_t)2, (seq_t)4, (seq_t)8,
        (seq_t)12, (seq_t)18, (seq_t)26, (seq_t)40,
        (seq_t)56
    };

    /* allocate space for at least the first nine terms */
    if (cap < 9) {
        size_t new_cap = 9;
        cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cap = new_cap;
    }

    /* copy the initial values */
    for (idx_t i = 0; i < 9; ++i)
        cache[i] = init[i];

    last = 8;    /* highest index computed */
}

/* ensure that the cache can hold indices up to n */
static void ensure_capacity(idx_t n)
{
    if (n < cap)
        return;

    size_t new_cap = cap ? cap * 2 : 8;
    while (new_cap <= (size_t)n)
        new_cap <<= 1;

    cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    cap = new_cap;
}

/* the requested sequence term */
seq_t a(idx_t n)
{
    /* initialise on the first call */
    if (last == (idx_t)-1)
        init_cache_for_a212585();

    /* nothing to compute if the value is already cached */
    if (n <= last)
        return cache[n];

    /* make sure we can store all needed indices */
    ensure_capacity(n);

    /* compute all missing terms up to n */
    for (idx_t i = last + 1; i <= n; ++i) {
        seq_t pos, neg, val;

        /* recurrence: a[i] = a[i-1] + 2·a[i-2] – 2·a[i-3] + a[i-5] – 2·a[i-6] + a[i-8] */
        /* build positive and negative parts separately to avoid unsigned underflow */
        pos = cache[i - 1] + (cache[i - 2] << 1) + cache[i - 5] + cache[i - 8];
        neg = (cache[i - 3] << 1) + (cache[i - 6] << 1);
        val = pos - neg;   /* pos ≥ neg for all i ≥ 9 */

        cache[i] = val;
    }

    last = n;
    return cache[n];
}
