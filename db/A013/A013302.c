/*
 * A013302  Expansion of e.g.f. cosh(log(x+1)-arctanh(x)) (even powers only).
 * https://oeis.org/A013302
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (4*n-5)*((2*n-3)!!)^2 + (2*n-3)^2*a(n-1) with a(0)=1,
 *            a(1)=0. Here ((2*n-3)!!) = product_{k=1}^{n-1} (2*k-1)
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 6, u64 n <= 10, u128 n <= 13,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

static seq_t *a_cache = NULL;   /* a values */
static seq_t *B_cache = NULL;   /* B[n] = (2*n-3)!! */
static idx_t cache_len = 0;     /* number of entries currently filled */

/* Ensure caches contain values up to index n (inclusive). */
static void ensure_idx(idx_t n)
{
    if (n < cache_len) return;

    idx_t new_len = n + 1;
    size_t new_size = new_len * sizeof(seq_t);

    a_cache = (seq_t *)realloc(a_cache, new_size);
    B_cache = (seq_t *)realloc(B_cache, new_size * sizeof(seq_t));

    /* In case of allocation failure, abort (build system guarantees success). */
    if (!a_cache || !B_cache) abort();

    for (idx_t i = cache_len; i < new_len; ++i)
    {
        if (i == 0)
        {
            a_cache[0] = (seq_t)1;
            B_cache[0] = (seq_t)1;  /* defined as 1 for convenience */
        }
        else if (i == 1)
        {
            a_cache[1] = (seq_t)0;
            B_cache[1] = (seq_t)1;  /* product over empty set */
        }
        else
        {
            /* B[i] = B[i-1] * (2*i-3) */
            seq_t mult = (seq_t)(2 * (seq_t)i - 3);
            B_cache[i] = B_cache[i - 1] * mult;

            /* a[i] = (4*i-5)*B[i]^2 + (2*i-3)^2 * a[i-1] */
            seq_t term1 = (seq_t)(4 * (seq_t)i - 5);
            term1 = term1 * B_cache[i] * B_cache[i];

            seq_t mult2 = (seq_t)(2 * (seq_t)i - 3);
            mult2 = mult2 * mult2 * a_cache[i - 1];

            a_cache[i] = term1 + mult2;
        }
    }
    cache_len = new_len;
}

seq_t a(idx_t n)
{
    ensure_idx(n);
    return a_cache[n];
}
