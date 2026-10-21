/*
 * A237938  Number of (n+1) X (1+1) 0..2 arrays with the maximum minus the
 *          minimum of every 2 X 2 subblock equal.
 * https://oeis.org/A237938
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 11, u64 n <= 23, u128 n <= 47,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A237938: Number of (n+1) × (1+1) 0..2 arrays with the
   maximum minus the minimum of every 2 × 2 subblock equal.
   Recurrence (n ≥ 8):
        a(n) = 8·a(n-1) – a(n-2) – 64·a(n-3) – 14·a(n-4)
               + 88·a(n-5) + 8·a(n-6) – 24·a(n-7)
   Initial values:
        a(1)=81, a(2)=437, a(3)=2417, a(4)=13893, a(5)=81245,
        a(6)=482409, a(7)=2893685 (one‑based indexing).  */
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Handle undefined n < 1 (no test requires this).  */
    if (n == (idx_t)0)
        return (seq_t)0;

    /* Static cache for computed terms, one‑based indexing */
    static seq_t *cache   = NULL;   /* cache[i] holds a(i+1)   */
    static size_t cap     = 0;      /* allocated capacity */
    static size_t sz      = 0;      /* number of terms stored */

    /* First initialization: store first seven terms  */
    if (cache == NULL) {
        cap = 16;
        cache = (seq_t*)malloc(cap * sizeof(seq_t));
        if (cache == NULL)
            return (seq_t)0;          /* allocation failure */

        cache[0] = (seq_t)81;           /* a(1) */
        cache[1] = (seq_t)437;          /* a(2) */
        cache[2] = (seq_t)2417;         /* a(3) */
        cache[3] = (seq_t)13893;        /* a(4) */
        cache[4] = (seq_t)81245;        /* a(5) */
        cache[5] = (seq_t)482409;       /* a(6) */
        cache[6] = (seq_t)2893685;      /* a(7) */
        sz = 7;
    }

    /* Return cached term if already computed  */
    if ((size_t)n <= sz)
        return cache[(size_t)n - 1];

    /* Grow cache to accommodate up to term n  */
    if (n > (idx_t)cap) {
        size_t new_cap = cap;
        while ((idx_t)new_cap < n)
            new_cap *= 2;
        seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (tmp == NULL)
            return (seq_t)0;          /* allocation failure */
        cache = tmp;
        cap   = new_cap;
    }

    /* Compute missing terms up to n */
    for (size_t k = sz; k < (size_t)n; ++k) {
        seq_t val = 0;
        val += (seq_t)8  * cache[k-1];
        val -= cache[k-2];
        val -= (seq_t)64 * cache[k-3];
        val -= (seq_t)14 * cache[k-4];
        val += (seq_t)88 * cache[k-5];
        val += (seq_t)8  * cache[k-6];
        val -= (seq_t)24 * cache[k-7];
        cache[k] = val;
    }

    sz = (size_t)n;
    return cache[(size_t)n - 1];
}
