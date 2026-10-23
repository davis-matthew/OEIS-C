/*
 * A189091  Zero-one sequence based on floor(sqrt(5)): a(A022839(k))=a(k);
 *          a(A108598(k))=1-a(k); a(1)=0.
 * https://oeis.org/A189091
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 1 For n > 1 let s = ⌊n/√5⌋. If n = ⌊(s+1)·√5⌋ then a(n) =
 *            a(s+1); else a(n) = 1 - a(n - s). (Here √5 is irrational; floor
 *            operations are performed with integers only.)
 * Range:     u8 n <= 6, u16 n <= 127, u32 n <= 135, u64 n <= 135,
 *            u128 n <= 135, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
#include "../utils/floor_sqrt.h"

static seq_t invalid_val = (seq_t)-1;           /* marks uncomputed entries */
static seq_t *cache         = NULL;            /* memoization cache   */
static size_t cache_cap    = 0;                /* current cache size  */

/* auxiliary: floor((s+1)*√5) = floor( (s+1)^2 * 5 )^0.5 */
static inline seq_t u_val(idx_t k)
{
    seq_t t = (seq_t)5 * (seq_t)k * (seq_t)k;   /* (k*√5)^2 = 5k^2   */
    return floor_sqrt(t);
}

/* floor(n/√5) using integer arithmetic: floor( sqrt( n^2 / 5 ) ) */
static inline idx_t floor_n_over_alpha(idx_t n)
{
    seq_t nsq   = (seq_t)n * (seq_t)n;          /* n^2  */
    seq_t div   = nsq / (seq_t)5;               /* n^2 / 5  */
    seq_t root  = floor_sqrt(div);              /* floor(sqrt(...)) */
    return (idx_t)root;
}

/* grow cache so that indices [0..n] are valid */
static void ensure_cache(idx_t n)
{
    if (n < cache_cap)
        return;

    size_t new_cap = cache_cap ? cache_cap * 2 : 1;
    while (new_cap <= n)
        new_cap *= 2;

    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp)
        abort();

    for (size_t i = cache_cap; i < new_cap; ++i)
        tmp[i] = invalid_val;

    cache = tmp;
    cache_cap = new_cap;
}

/* recursive definition with memoisation */
static seq_t rec_a(idx_t n)
{
    if (n == 1)
        return (seq_t)1;

    if (cache[n] != invalid_val)
        return cache[n];

    idx_t s  = floor_n_over_alpha(n);          /* s = ⌊n/√5⌋ */
    idx_t k1 = s + 1;                          /* candidate index in u */

    seq_t u_val_k1 = u_val(k1);

    seq_t result;
    if (u_val_k1 == (seq_t)n) {
        /* n is in u */
        result = rec_a(k1);
    } else {
        /* n is in v */
        result = ((seq_t)1) - rec_a(n - s);
    }

    cache[n] = result;
    return result;
}

seq_t a(idx_t n)
{
    if (n == 0)
        abort();                               /* offset 1, n ≥ 1 */

    ensure_cache(n);
    return rec_a(n);
}
