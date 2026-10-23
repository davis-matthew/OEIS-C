/*
 * A041522  Numerators of continued fraction convergents to sqrt(278).
 * https://oeis.org/A041522
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 14, u64 n <= 29, u128 n <= 61,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A041522 – numerators of continued‑fraction convergents to sqrt(278).
   Recurrence:
     p_{-2} = 0,  p_{-1} = 1
     for k ≥ 0 :  p_k = a_k * p_{k-1} + p_{k-2}
     a_0 = 16
     a_1..a_6 = [1, 2, 16, 2, 1, 32]   (period 6)
   The sequence a(n) is p_n, with a(0)=16, a(1)=17, a(2)=50, … */

/* --------------------------------------------------------------------- */

#include <stdlib.h>
#include <stdint.h>

/* static cache for computed numerators */
static seq_t *cache = NULL;   /* array of computed p_k */
static size_t cache_size   = 0;   /* number of elements stored in cache */
static size_t cache_cap    = 0;   /* allocated capacity of cache */

/* period of continued‑fraction partial quotients (excluding a_0) */
static const int period[6] = { 1, 2, 16, 2, 1, 32 };

/* ensure the cache can hold at least 'needed' elements */
static void ensure_capacity(size_t needed)
{
    if (needed <= cache_cap)
        return;
    size_t new_cap = cache_cap ? cache_cap : 16;
    while (new_cap < needed)
        new_cap <<= 1;

#ifdef __cplusplus
    seq_t *newp = new seq_t[new_cap];
    if (cache) {
        for (size_t i = 0; i < cache_size; ++i)
            newp[i] = cache[i];
        delete[] cache;
    }
    cache = newp;
#else
    seq_t *newp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!newp) {
        /* allocation failed – terminate (caller may handle differently) */
        exit(1);
    }
    cache = newp;
#endif
    cache_cap = new_cap;
}

/* return the n‑th term (a(0)=16) of the sequence */
seq_t a(idx_t n)
{
    /* work with size_t for indexing; n is guaranteed non‑negative */
    size_t idx = (size_t)n;

    /* start the cache with a(0) if it is empty */
    if (cache_size == 0) {
        ensure_capacity(1);
        cache[0] = (seq_t)16;   /* p_0 */
        cache_size = 1;
    }

    /* compute missing terms up to idx */
    for (size_t i = cache_size; i <= idx; ++i) {
        seq_t val;
        if (i == 0) {
            val = (seq_t)16;   /* already computed */
        } else {
            /* partial quotient a_i */
            int a_i;
            if (i == 0)
                a_i = 16;   /* unused here because i>0 */
            else if (i == 1)
                a_i = period[(1 - 1) % 6];   /* a_1 = period[0] = 1 */
            else
                a_i = period[(i - 1) % 6];

            if (i == 1) {
                /* p_1 = a_1 * p_0 + p_{-1} ; p_{-1} = 1 */
                val = (seq_t)a_i * cache[0] + (seq_t)1;
            } else {
                /* p_i = a_i * p_{i-1} + p_{i-2} */
                val = (seq_t)a_i * cache[i - 1] + cache[i - 2];
            }
        }

        /* store the computed value, expanding the array if necessary */
        ensure_capacity(i + 1);
        cache[i] = val;
        ++cache_size;
    }

    return cache[idx];
}
