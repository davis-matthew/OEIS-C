/*
 * A098750  a(n+1) = a(n) + 10's complement of first digit of a(n); a(0) = 0.
 * https://oeis.org/A098750
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 45, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A098750: a(0)=0; for n>=0,
   a(n+1) = a(n) + (10 - first_digit(a(n)))         */
/* The first digit is the most significant decimal digit.  */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

/* Helper: return the most significant decimal digit of v. */
static inline seq_t first_digit(seq_t v)
{
    while (v >= (seq_t)10)
        v /= (seq_t)10;
    return v;
}

/* Compute a(n) for any n >= 0, memoizing intermediate results. */
seq_t a(idx_t n)
{
    static seq_t *cache = NULL;       /* array holding a(0)..a(cache_len-1) */
    static idx_t cache_len = 0;       /* number of computed entries */

    /* Ensure the cache can hold up to n. */
    if (cache_len <= n) {
        size_t new_len = (size_t)(n + 1);
        /* Reallocate cache if necessary. */
        if (!cache)
            cache = (seq_t *)malloc(new_len * sizeof(seq_t));
        else
            cache = (seq_t *)realloc(cache, new_len * sizeof(seq_t));

        /* Initialize new entries. */
        if (cache_len == 0)           /* first-call initialization: a(0)=0 */
            cache[0] = (seq_t)0;
        for (idx_t k = cache_len; k <= n; ++k) {
            if (k == 0)
                continue;              /* a(0) already set */
            seq_t prev = cache[k - 1];
            seq_t msd = first_digit(prev);
            seq_t inc = (seq_t)10 - msd;
            cache[k] = prev + inc;
        }
        cache_len = new_len;
    }
    return cache[n];
}
