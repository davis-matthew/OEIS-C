/*
 * A116801  Number of permutations of length n which avoid the patterns 1234,
 *          1432, 2413.
 * https://oeis.org/A116801
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 10, u32 n <= 20, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * OEIS A116801 – Number of permutations of length n which avoid the
 * patterns 1234, 1432, 2413.
 *
 * Recurrence (for n ≥ 9):
 *   a(n) = 7*a(n-1) - 18*a(n-2) + 24*a(n-3) - 19*a(n-4)
 *          + 7*a(n-5) - 4*a(n-6) + 1*a(n-7) - 1*a(n-8)
 *
 * Initial values:
 *   a(1)=1, a(2)=2, a(3)=6, a(4)=21, a(5)=71,
 *   a(6)=229, a(7)=726, a(8)=2299
 *
 * The implementation memoises a table up to the largest n requested,
 * so repeated calls either increase in size or reuse previously
 * computed values.  All arithmetic is performed on type seq_t
 * (which may be 8/16/32/64-bit unsigned or arbitrary precision),
 * using only the operators permitted in the problem statement.
 */

#include <stdlib.h>

static seq_t *cache = NULL;          /* memoisation table (index 0 unused) */
static idx_t cache_size = 0;         /* number of indices allocated minus 1 */
static idx_t known_max   = 0;        /* largest index whose value is known */

/* Ensure cache can hold indices up to n (inclusive).  Grow geometrically. */
static void ensure_cache(idx_t n)
{
    if (cache_size > n) return;
    idx_t new_size = cache_size > 0 ? cache_size : 16;
    while (new_size <= n) new_size <<= 1;
    seq_t *tmp = (seq_t *)realloc(cache, (new_size + 1) * sizeof(seq_t));
    if (!tmp) { free(cache); abort(); }
    cache      = tmp;
    cache_size = new_size;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;                       /* safety for n < 1 */

    /* initialise first 8 terms on the first call */
    if (known_max == 0) {
        ensure_cache(8);                        /* allocate space */
        cache[0] = 0;                           /* unused (index 1‑based) */
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)2;
        cache[3] = (seq_t)6;
        cache[4] = (seq_t)21;
        cache[5] = (seq_t)71;
        cache[6] = (seq_t)229;
        cache[7] = (seq_t)726;
        cache[8] = (seq_t)2299;
        known_max = 8;
    }

    if (n <= known_max) return cache[n];

    /* grow cache to at least n */
    ensure_cache(n);

    /* compute new values up to n */
    for (idx_t i = known_max + 1; i <= n; ++i) {
        seq_t pos = 7 * cache[i-1] + 24 * cache[i-3]
                    + 7 * cache[i-5] + cache[i-7];
        seq_t neg = 18 * cache[i-2] + 19 * cache[i-4]
                    + 4  * cache[i-6] + cache[i-8];
        cache[i] = pos - neg;                   /* pos ≥ neg for all n ≥ 9 */
    }
    known_max = n;

    return cache[n];
}
