/*
 * A203473  a(n) = v(n+1)/v(n), where v=A203472.
 * https://oeis.org/A203473
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = RisingFactorial(6 + n, n) = (2*n+5)! / (n+5)!
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 13, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *a_cache = NULL;  /* indices start at 1, a_cache[0] unused */
static idx_t  a_cache_len = 0; /* highest index computed */

/* Ensure the cache is initialized. */
static void
a_cache_init(void)
{
    if (a_cache != NULL)
        return;
    a_cache = (seq_t *)calloc((size_t)2, sizeof(seq_t)); /* indices 0 and 1 */
    if (!a_cache)
        exit(1);
    a_cache[1] = (seq_t)7;          /* a(1) = 7 */
    a_cache_len = 1;
}

/* Compute a(n) using the recurrence
 *   a(k) = a(k-1) * (2*k+4) * (2*k+5) / (k+5)
 * The division is exact; we perform it first to avoid overflow. */
seq_t
a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;   /* undefined for n = 0, return 0 */

    a_cache_init();

    if ((idx_t)n <= a_cache_len)
        return a_cache[n];

    /* enlarge cache to hold indices up to n */
    size_t new_size = (size_t)(n + 1);   /* indices 0..n */
    seq_t *tmp = (seq_t *)realloc(a_cache, new_size * sizeof(seq_t));
    if (!tmp) {
        free(a_cache);
        exit(1);
    }
    a_cache = tmp;

    for (idx_t k = a_cache_len + 1; k <= n; ++k) {
        seq_t prev = a_cache[k - 1];
        seq_t part = prev / (k + 5);        /* exact division */
        part = part * (2 * k + 4);          /* multiply by (2k+4) */
        part = part * (2 * k + 5);          /* multiply by (2k+5) */
        a_cache[k] = part;
    }

    a_cache_len = n;
    return a_cache[n];
}
