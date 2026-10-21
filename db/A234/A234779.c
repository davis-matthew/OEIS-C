/*
 * A234779  Number of (n+1) X (1+1) 0..3 arrays with no adjacent elements
 *          equal and with each 2 X 2 subblock having the number of clockwise
 *          edge increases equal to the number of counterclockwise edge
 *          increases.
 * https://oeis.org/A234779
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 7*a(n-1) – 4*a(n-2) with a(1)=76, a(2)=484
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 10, u64 n <= 22, u128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* static cache to allow calls in any order */
static seq_t *cache = NULL;   /* cache[0] = a(1) */
static size_t cap = 0;        /* allocated capacity (# of entries) */
static idx_t max_n = 0;       /* largest n computed so far */

static void ensure_cache(idx_t n)
{
    /* we need to store a(n) at index n-1 */
    size_t needed = (size_t)n;
    if (needed <= cap)
        return;
    size_t new_cap = cap > 0 ? cap : 4;
    while (new_cap < needed)
        new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp)
        abort();                 /* out of memory */
    cache = tmp;
    cap = new_cap;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                /* below offset behaviour is unspecified */

    ensure_cache(n);

    /* seed initial values on first call */
    if (max_n == 0) {
        cache[0] = (seq_t)76;   /* a(1) */
        cache[1] = (seq_t)484;  /* a(2) */
        max_n = 2;
    }

    if (n <= max_n)
        return cache[n - 1];

    /* compute values up to n */
    for (idx_t i = max_n + 1; i <= n; ++i) {
        seq_t val = ((seq_t)7) * cache[i - 2] - ((seq_t)4) * cache[i - 3];
        cache[i - 1] = val;
    }
    max_n = n;
    return cache[n - 1];
}
