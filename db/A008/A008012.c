/*
 * A008012  Coordination sequence T1 for Zeolite Code AFG.
 * https://oeis.org/A008012
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 11, u16 n <= 177, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   alloc_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A008012 – Coordination sequence T1 for Zeolite Code AFG
 *
 *  The generating function is rational with a denominator of degree 21.
 *  This leads to a linear recurrence of order 21:
 *
 *      a(n) = a(n‑1) + 2a(n‑5) – 2a(n‑6) – 2a(n‑10) + 2a(n‑11)
 *             + 2a(n‑15) – 2a(n‑16) – a(n‑20) + a(n‑21)  + r(n),
 *
 *  where r(n) is the coefficient of xⁿ in the numerator. The only
 *  non‑zero value of r(n) is r(21)=1; for n≠21, r(n)=0.
 *
 *  The first 21 terms are provided in the OEIS entry and are used as
 *  initial conditions.
 *
 *  The function stores computed terms in a static cache so that calls
 *  with arbitrary order of indices remain O(n) total per maximum n.
 */

#include <stddef.h>
#include <string.h>
#include "../utils/alloc_seq.h"

static const seq_t init_vals[21] = {
    1,   4,  10,  20,  34,  53,  76, 103, 135, 170,
  208, 250, 299, 355, 412, 469, 530, 600, 678, 756, 834
};

static const seq_t rhs_21 = (seq_t)1;

static seq_t *cache = NULL;
static size_t cache_len = 0;

/* ------------------------------------------------------------------ */
/*  a:  compute a(n) in the OEIS coordination sequence.               */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    /* initialise cache on first call */
    if (cache_len == 0) {
        cache_len = 21;
        cache     = alloc_seq(cache_len);
        memcpy(cache, init_vals, cache_len * sizeof(seq_t));
    }

    if ((size_t)n < cache_len)
        return cache[(size_t)n];

    /* extend cache up to n */
    size_t old_len = cache_len;
    size_t new_len = (size_t)n + 1;
    seq_t *new_cache = alloc_seq(new_len);
    memcpy(new_cache, cache, old_len * sizeof(seq_t));
    free(cache);
    cache = new_cache;
    cache_len = new_len;

    for (size_t i = old_len; i <= (size_t)n; ++i) {
        if (i < 21) {
            cache[i] = init_vals[i];
            continue;
        }

        seq_t val = cache[i-1];
        val += (seq_t)2 * cache[i-5];
        val -= (seq_t)2 * cache[i-6];
        val -= (seq_t)2 * cache[i-10];
        val += (seq_t)2 * cache[i-11];
        val += (seq_t)2 * cache[i-15];
        val -= (seq_t)2 * cache[i-16];
        val -= cache[i-20];
        val += cache[i-21];

        if (i == 21)
            val += rhs_21;
        /* for i > 21, rhs is zero */

        cache[i] = val;
    }

    return cache[(size_t)n];
}
