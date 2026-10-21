/*
 * A229690  Number of defective 3-colorings of an nX4 0..2 array connected
 *          horizontally and antidiagonally with exactly two mistakes, and
 *          colors introduced in row-major 0..2 order.
 * https://oeis.org/A229690
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 27*a(n-1) - 243*a(n-2) + 729*a(n-3) for n>4 Initial
 *            values: a(1)=3, a(2)=245, a(3)=5077, a(4)=80757
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 8, u64 n <= 17, u128 n <= 37,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;
static idx_t cache_len = 0;   /* highest index currently stored (0 means none) */

static void ensure_cache(idx_t n)
{
    if (n <= cache_len) return;                /* already computed */

    /* extend the allocation */
    seq_t *tmp = (seq_t*)realloc(cache, (n + 1) * sizeof(seq_t));
    if (!tmp) abort();                          /* out of memory */
    cache = tmp;

    /* initialise new slots (not strictly needed, but safe) */
    for (idx_t i = cache_len + 1; i <= n; ++i) cache[i] = 0;

    /* set the first four values if not yet set */
    if (n >= 1 && cache[1] == 0) cache[1] = 3;
    if (n >= 2 && cache[2] == 0) cache[2] = 245;
    if (n >= 3 && cache[3] == 0) cache[3] = 5077;
    if (n >= 4 && cache[4] == 0) cache[4] = 80757;

    /* compute any new terms using the recurrence */
    idx_t start = cache_len < 5 ? 4 : cache_len;   /* first index whose successor needs computation */
    for (idx_t k = start + 1; k <= n; ++k) {
        cache[k] = ((seq_t)27) * cache[k-1] + ((seq_t)729) * cache[k-3];
        cache[k] -= ((seq_t)243) * cache[k-2];
    }

    cache_len = n;
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;                       /* undefined for n<1, return 0 safely */
    ensure_cache(n);
    return cache[n];
}
