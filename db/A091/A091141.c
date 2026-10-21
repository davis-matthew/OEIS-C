/*
 * A091141  a(n) = 2*a(n-1) + 4*a(n-2) - 2*a(n-3) with initial terms 1, 4, 13.
 * https://oeis.org/A091141
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*a(n-1) + 4*a(n-2) - 2*a(n-3) with a(1)=1, a(2)=4,
 *            a(3)=13 Sequence is memoised in a 0‑based array; a(1) is stored
 *            at index 0
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 20, u64 n <= 40, u128 n <= 79,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>  /* malloc, realloc, abort */

static seq_t *cache = NULL;  /* cache[0] = a(1) */
static size_t cap   = 0;     /* currently allocated elements */
static size_t len   = 0;     /* number of terms computed */

static void ensure_capacity(idx_t n)
{
    /* Ensure array can hold indices 0 .. n-1 (i.e. n elements). */
    if ((size_t)n <= cap)
        return;

    size_t newcap = cap ? cap * 2 : 8;
    if (newcap < (size_t)n)
        newcap = (size_t)n;

    /* realloc may return NULL on failure – abort on that case. */
    seq_t *tmp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
    if (!tmp)
        abort();
    cache = tmp;
    cap   = newcap;
}

static void init_cache(void)
{
    if (len == 0) {
        ensure_capacity(3);
        cache[0] = 1;   /* a(1) */
        cache[1] = 4;   /* a(2) */
        cache[2] = 13;  /* a(3) */
        len = 3;
    }
}

seq_t a(idx_t n)
{
    if (n < 1)
        return 0;   /* undefined value for out‑of‑range index */

    init_cache();

    if ((size_t)n <= len)
        return cache[(size_t)n - 1];

    ensure_capacity(n);

    /* Compute missing terms, iterating from len+1 up to n. */
    for (idx_t i = (idx_t)len + 1; i <= n; ++i) {
        /* Indices for the recurrence (0‑based in cache): */
        seq_t a1 = cache[(size_t)i - 2];  /* a(i-1) */
        seq_t a2 = cache[(size_t)i - 3];  /* a(i-2) */
        seq_t a3 = cache[(size_t)i - 4];  /* a(i-3) */
        seq_t val = 2 * a1 + 4 * a2 - 2 * a3;
        cache[(size_t)i - 1] = val;
    }

    len = n;
    return cache[(size_t)n - 1];
}
