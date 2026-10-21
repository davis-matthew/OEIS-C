/*
 * A052245  Expansion of 10*x / ((1 - x) * (1 - 10*x)^2) in powers of x.
 * https://oeis.org/A052245
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = n*10^n + a(n-1), a(0) = 0 (equivalent to the closed form
 *            in OEIS)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 17, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* static cache of computed terms and powers of 10 */
static seq_t *cache = NULL;   /* a(i) values */
static seq_t *pow10 = NULL;   /* 10^i values */
static idx_t cached = 0;      /* number of cached entries (cache[0..cached-1] exist) */

seq_t a(idx_t n)
{
    /* initialise cache for n==0 */
    if (cached == 0) {
        cached = 1;
        cache  = (seq_t *)malloc(sizeof(seq_t));
        pow10  = (seq_t *)malloc(sizeof(seq_t));
        cache[0] = (seq_t)0;
        pow10[0] = (seq_t)1;
        if (!cache || !pow10) return (seq_t)0; /* allocation failure – return 0 */
    }

    /* return if already computed */
    if (n < cached) return cache[n];

    /* grow the arrays to hold entries up to n */
    idx_t newsize = n + 1;
    if (newsize < 2 * cached) newsize = 2 * cached;
    cache = (seq_t *)realloc(cache, newsize * sizeof(seq_t));
    pow10 = (seq_t *)realloc(pow10, newsize * sizeof(seq_t));
    if (!cache || !pow10) return (seq_t)0; /* allocation failure – return 0 */

    /* compute missing entries */
    for (idx_t i = cached; i <= n; ++i) {
        pow10[i] = pow10[i - 1] * (seq_t)10;
        cache[i] = ((seq_t)i * pow10[i]) + cache[i - 1];
    }
    cached = n + 1;               /* we now have entries 0..n cached */
    return cache[n];
}
