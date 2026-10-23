/*
 * A173125  a(n) = Sum_{k == floor(n/2) (mod 5)} C(n,k).
 * https://oeis.org/A173125
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 17, u32 n <= 33, u64 n <= 34, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = 2*a(n-1)+3*a(n-2)-6*a(n-3)-a(n-4)+2*a(n-5) */
/* Initial values: a(0)=1, a(1)=1, a(2)=2, a(3)=3, a(4)=6 */

#include <stdlib.h>

static seq_t *cache   = NULL;       /* array of computed terms */
static idx_t  cap     = 0;         /* allocated capacity (indices 0..cap-1) */
static idx_t  stored  = 0;         /* number of terms already stored (indices 0..stored-1) */

static void
ensure_capacity(idx_t n)
{
    if (cap > n)
        return; /* already large enough */

    idx_t new_cap = (cap > 0) ? cap : (idx_t)64;
    while (new_cap <= n)
        new_cap <<= 1;

    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) abort();            /* allocation failed – terminate */
    cache = tmp;
    cap   = new_cap;
}

seq_t
a(idx_t n)
{
    ensure_capacity(n);

    /* Compute missing terms up to index n */
    for (idx_t i = stored; i <= n; ++i) {
        if (i == 0)
            cache[i] = (seq_t)1;
        else if (i == 1)
            cache[i] = (seq_t)1;
        else if (i == 2)
            cache[i] = (seq_t)2;
        else if (i == 3)
            cache[i] = (seq_t)3;
        else if (i == 4)
            cache[i] = (seq_t)6;
        else {
            /* Recurrence: 2*a(i-1)+3*a(i-2)-6*a(i-3)-a(i-4)+2*a(i-5) */
            seq_t pos = 2 * cache[i-1] + 3 * cache[i-2] + 2 * cache[i-5];
            seq_t neg = 6 * cache[i-3] + cache[i-4];
            cache[i] = (pos >= neg) ? (pos - neg) : (seq_t)0;  /* to avoid underflow */
        }
    }
    stored = n + 1;  /* indices 0..n are now stored */
    return cache[n];
}
