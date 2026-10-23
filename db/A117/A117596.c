/*
 * A117596  Start with x=6/5; repeatedly apply the map x -> x*ceiling(x);
 *          sequence gives numerators of the resulting sequence of fractions.
 * https://oeis.org/A117596
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 6, u64 n <= 7, u128 n <= 8,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A117596: define a(1)=6.  For n>1 let d_{n-1} be the denominator of the
   fraction having numerator a(n-1) (initially 5).  Then
   ceil(x_{n-1}) = (a(n-1)+d_{n-1}-1)/d_{n-1}.
   a(n) = a(n-1) * ceil(x_{n-1});  If a(n) is divisible by d_{n-1} set d_n=1,
   else keep d_n=d_{n-1}.  The function returns a(n).  The sequence is memoized
   to permit arbitrary call order. */

#include <stdlib.h>

static seq_t *cache = NULL;        /* array of numerators */
static idx_t cache_size = 0;       /* allocated size */
static idx_t cache_used = 0;       /* highest n computed so far */
static seq_t  num_last = 6;        /* last computed numerator */
static seq_t  denom_last = 5;      /* last denominator (1 after first integer) */

seq_t a(idx_t n)
{
    /* Lazy allocation: allocate before we need any terms. */
    if (n == 0) return 0;                /* undefined for n<1, keep safe */

    if (cache_used == 0) {
        /* Initial state: a(1)=6, denominator=5. */
        cache_size = n;
        cache = (seq_t *)malloc(sizeof(seq_t) * cache_size);
        cache[0] = num_last;            /* a(1)=6 */
        cache_used = 1;
    } else if (cache_size < n) {
        /* Grow the cache if needed. */
        cache = (seq_t *)realloc(cache, sizeof(seq_t) * n);
        cache_size = n;
    }

    /* If the requested index is already known, return it. */
    if (n <= cache_used)
        return cache[n - 1];

    /* Compute missing terms up to n. */
    for (idx_t k = cache_used + 1; k <= n; ++k) {
        /* ceil(x) = (num + denom - 1) / denom. */
        seq_t ceil_val = (num_last + denom_last - 1) / denom_last;
        num_last = num_last * ceil_val;
        cache[k - 1] = num_last;

        /* Update denominator: once it becomes 1 it stays 1. */
        if (denom_last != 1 && (num_last % denom_last) == 0)
            denom_last = 1;
    }

    cache_used = n;
    return cache[n - 1];
}
