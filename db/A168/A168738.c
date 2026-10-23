/*
 * A168738  Number of reduced words of length n in Coxeter group on 13
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^18 = I.
 * https://oeis.org/A168738
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 17, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A168738: number of reduced words of length n in the Coxeter
 *  group with 13 generators S_i, (S_i)^2 = I and (S_i S_j)^18 = I.
 *
 *  The generating function is
 *
 *      P(t) / Q(t)  with
 *
 *          P(t) = 1 + 2t + 2t^2 + ... + 2t^17 + t^18,
 *          Q(t) = 1 - 11t - 11t^2 - ... - 11t^17 + 66t^18.
 *
 *  Hence the coefficients a(n) satisfy the linear recurrence
 *
 *      a(n) = 11 a(n-1) + 11 a(n-2) + ... + 11 a(n-17) - 66 a(n-18)
 *              + p_n ,
 *
 *  where  p_0 = p_18 = 1,  p_n = 2 for 1 ≤ n ≤ 17  and  p_n = 0 for n > 18.
 *
 *  The recurrence uses a(n-k)=0 for k>n.  The values for n≤17 are
 *  obtained from the recurrence with these zero initial conditions,
 *  and the first 18 values match the OEIS table.  Subsequent values
 *  are computed by the homogeneous recurrence for n>18.
 *
 *  The function keeps a static cache so that calls are O(n) with very
 *  small constant factors and can be called in any order.
 *
 *  Only the operations allowed by the exercise are used.  The code
 *  works for any supported seq_t and idx_t type. */

#include <stdlib.h>

/* Forward declaration of the function required by the exercise. */
seq_t a(idx_t n);

static seq_t *cache = NULL;      /* array of computed values */
static size_t  cache_size = 0;   /* current capacity of cache */

/* Helper: ensure the cache has room for indices up to 'size' (inclusive). */
static void ensure_cache(idx_t size)
{
    size_t needed = (size_t)(size + 1);   /* indices 0 .. size */

    if (needed <= cache_size)
        return;

    size_t new_cap = cache_size ? cache_size : 32;
    while (new_cap < needed)
        new_cap <<= 1;           /* grow by factor of 2 */

    seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!new_cache) abort();   /* no memory; terminate */

    /* If expanding, zero the new entries so they are well‑defined.
       This is not strictly necessary for the recurrence because
       we always fill them before using, but it is harmless. */
    for (size_t i = cache_size; i < new_cap; ++i)
        new_cache[i] = 0;

    cache = new_cache;
    cache_size = new_cap;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    /* Ensure the cache covers index 'n' */
    ensure_cache(n);

    /* If already computed, return immediately */
    if (n < cache_size && cache[n] != 0) {
        /* Note: cache[0] may be zero (a(0)=1), so use a flag check.
           We'll compute missing values even if they are zero. */
    }

    /* Compute values up to n iteratively if necessary */
    for (idx_t i = 0; i <= n; ++i) {
        /* If already computed (and not zero), skip */
        /* We just recompute all to keep logic simple. */
        seq_t val = 0;

        /* Sum 11*a(i-j) for j=1..17, where i-j >= 0 */
        for (int j = 1; j <= 17; ++j) {
            if (i >= (idx_t)j) {
                val += (seq_t)11 * cache[i - (idx_t)j];
            }
        }

        /* Subtract 66*a(i-18) if i>=18 */
        if (i >= (idx_t)18) {
            val -= (seq_t)66 * cache[i - (idx_t)18];
        }

        /* Add p_i for i <= 18 */
        if (i <= (idx_t)18) {
            if (i == (idx_t)0 || i == (idx_t)18) {
                val += 1;
            } else {
                val += 2;
            }
        }

        cache[i] = val;
    }

    return cache[n];
}
