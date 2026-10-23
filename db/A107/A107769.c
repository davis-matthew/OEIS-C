/*
 * A107769  a(n) = (A001333(n+1) - 2*A005409(floor((n+3)/2)) - 1) / 4.
 * https://oeis.org/A107769
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (A001333(n+1) - 2*A005409(floor((n+3)/2)) - 1) / 4
 *            Recurrence (obtained from the g.f.): a(n) =
 *            3*a(n-1)+a(n-2)-7*a(n-3) +3*a(n-4)-a(n-5)+a(n-6)+a(n-7) (n≥7)
 * Range:     u8 n <= 6, u16 n <= 12, u32 n <= 25, u64 n <= 50, u128 n <= 101,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Includes needed for memory allocation and exit.  The types seq_t and idx_t
   are supplied by the build system; no other definitions are required. */
#include <stdlib.h>

/* Initial seven terms of the sequence.  They are read‑only constants. */
static const seq_t init[7] = { (seq_t)0, (seq_t)1, (seq_t)2, (seq_t)8,
                               (seq_t)19, (seq_t)54, (seq_t)130 };

/* Cache of computed terms.  cache[i] contains a(i) for 0 ≤ i < cache_len. */
static seq_t *cache      = NULL;   /* pointer to allocated storage   */
static size_t cache_cap  = 0;       /* number of elements actually
                                       allocated	                */
static size_t cache_len  = 0;       /* number of elements that have
                                       already been filled          */

/* Ensure that storage for at least indices 0…need is available.
   Newly allocated elements are initialized with the first seven
   terms of the sequence. */
static void ensure_capacity(idx_t need)
{
    if (cache_cap > (size_t)need) return;     /* already large enough */

    size_t new_cap = (cache_cap == 0) ? 8 : cache_cap * 2;
    while (new_cap <= (size_t)need) new_cap <<= 1;

    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) exit(EXIT_FAILURE);            /* fatal: out of memory */

    cache      = tmp;
    cache_cap  = new_cap;

    /* initialise the first seven terms if they have not yet been set   */
    if (cache_len < 7) {
        for (size_t i = cache_len; i < 7; ++i)
            cache[i] = init[i];
        cache_len = 7;                     /* we now have 0…6 filled       */
    }
}

/* Return a(n).  The result is cached so that repeated calls are
   fast and calls come in arbitrary order. */
seq_t a(idx_t n)
{
    /* make sure we have storage for the required index */
    ensure_capacity(n);

    /* value already computed? */
    if ((idx_t)cache_len > n)
        return cache[(size_t)n];

    /* compute missing terms up to n */
    for (idx_t i = (idx_t)cache_len; i <= n; ++i) {
        if (i < 7) {
            cache[(size_t)i] = init[(size_t)i];
            continue;
        }

        /* recurrence:  a(i) = 3*a(i-1)+a(i-2)-7*a(i-3)+3*a(i-4)-a(i-5)+a(i-6)+a(i-7) */
        seq_t pos = (seq_t)3 * cache[(size_t)(i-1)] + cache[(size_t)(i-2)]
                    + (seq_t)3 * cache[(size_t)(i-4)] + cache[(size_t)(i-6)]
                    + cache[(size_t)(i-7)];
        seq_t neg = (seq_t)7 * cache[(size_t)(i-3)] + cache[(size_t)(i-5)];
        cache[(size_t)i] = pos - neg;
    }

    cache_len = (size_t)(n + 1);
    return cache[(size_t)n];
}
