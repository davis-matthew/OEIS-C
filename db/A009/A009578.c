/*
 * A009578  E.g.f. sinh(log(1+x))/exp(x). Unsigned sequence gives degrees of
 *          (finite by nilpotent) representations of Braid groups.
 * https://oeis.org/A009578
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   not stated
 * Range:     i8 n <= 4, i16 n <= 7, i32 n <= 12, i64 n <= 20, i128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A009578
 * Recurrence: a(0)=0
 *            a(n)= n * a(n-1) + n*(3 - n)/2   for n>0
 *    (the sign of a(n) automatically follows from the recurrence)
 */

#include <stdlib.h> /* malloc, free, realloc */

static seq_t *cache      = NULL;   /* cache of unsigned intermediate values */
static size_t cache_len  = 0;      /* length of valid cache entries  */
static size_t cache_cap  = 0;      /* allocated capacity */

seq_t a(idx_t n)
{
    /* a(0) is a special case: it is defined to be 0. */
    if (n == 0)
        return 0;

    /* Grow the cache if we need values up to n. */
    if ((size_t)n >= cache_len) {
        size_t new_cap = (size_t)n + 1;   /* we need indices 0..n */

        if (new_cap > cache_cap) {
            /* realloc returns void*, cast to seq_t* */
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            /* In case realloc fails we keep the old pointer; the problem
               statement guarantees sufficient memory. */
            cache = tmp;
            cache_cap = new_cap;
        }

        /* Fill cache from the current length up to n */
        for (size_t i = cache_len; i <= (size_t)n; ++i) {
            if (i == 0) {
                cache[0] = 0;   /* not used for n>0 but keeps cache clean */
            } else {
                seq_t prev   = cache[i - 1];
                seq_t mul    = (seq_t)i * prev;
                seq_t coeff  = (seq_t)i * ((seq_t)3 - (seq_t)i);
                seq_t term   = mul + coeff / 2;
                cache[i] = term;
            }
        }

        cache_len = (size_t)n + 1;
    }

    /* The recurrence already includes the sign.
       The sequence alternates sign: n even -> negative, n odd -> positive. */
    if ((n & 1U) == 0)
        return -cache[(size_t)n];
    else
        return cache[(size_t)n];
}
