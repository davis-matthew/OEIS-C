/*
 * A033140  Base-4 digits are, in order, the first n terms of the periodic
 *          sequence with initial period 1,0,0.
 * https://oeis.org/A033140
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = floor(4^(n+2)/63)
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 32, u128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*             Recurrence: a(n) = 4*a(n-1) + a(n-3) - 4*a(n-4)
 *                             a(1)=1, a(2)=4, a(3)=16, a(4)=65
 *   (indices are 1‑based as specified in the OEIS entry) */

#include <stdlib.h>   /* for malloc, realloc */

/* static cache for memoisation */
static seq_t *cache = NULL;     /* stores a(1) … a(current size) */
static idx_t  cap   = 0;        /* allocated capacity */
static idx_t  size  = 0;        /* number of stored values */

/* enlarge the cache to hold at least ‘need’ entries */
static void grow_cache(idx_t need)
{
    if (need <= cap) return;          /* already big enough */

    idx_t newcap = cap ? cap << 1 : 4;
    while (newcap < need) newcap <<= 1;

    /* reallocate and cast the void* returned by realloc */
    seq_t *tmp = (seq_t*)realloc(cache, newcap * sizeof(seq_t));
    cache = tmp;
    cap   = newcap;
}

/* Return the n‑th term of A033140 (1‑based).  Undefined for n<1. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* the sequence is defined only for n>=1 */

    /* initialise the cache on first use */
    if (!cache) {
        grow_cache(4);
        cache[0] = (seq_t)1;   /* a(1) */
        cache[1] = (seq_t)4;   /* a(2) */
        cache[2] = (seq_t)16;  /* a(3) */
        cache[3] = (seq_t)65;  /* a(4) */
        size    = 4;
    }

    /* if the term is already cached, return it */
    if (n <= size) return cache[n-1];

    /* grow the cache enough to hold all terms up to n */
    if (n > size) grow_cache(n);

    /* compute missing terms */
    for (idx_t i = size + 1; i <= n; ++i) {
        seq_t val;
        if (i <= 4) {         /* base cases – already set on first call   */
            switch (i) {
                case 1: val = (seq_t)1;  break;
                case 2: val = (seq_t)4;  break;
                case 3: val = (seq_t)16; break;
                case 4: val = (seq_t)65; break;
            }
        } else {
            /* a(i) = 4*a(i-1) + a(i-3) - 4*a(i-4) */
            val = (seq_t)4 * cache[i-2]      /* 4*a(i-1)           */
                + cache[i-4]                 /* + a(i-3)            */
                - (seq_t)4 * cache[i-5];     /* - 4*a(i-4)          */
        }
        cache[i-1] = val;
    }

    size = n;
    return cache[n-1];
}
