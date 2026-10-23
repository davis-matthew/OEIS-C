/*
 * A163723  Number of n X 2 binary arrays with all 1s connected, a path of 1s
 *          from left column to right column, and no 1 having more than two 1s
 *          adjacent.
 * https://oeis.org/A163723
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 17, u32 n <= 40, u64 n <= 86, u128 n <= 100,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A163723
   Recurrence (valid for n ≥ 8):
     a(n) = 4*a(n-1) - 5*a(n-2) + a(n-3) + 2*a(n-4) - a(n-5)
   Initial values:
     a(1)=1, a(2)=7, a(3)=21, a(4)=49, a(5)=101,
     a(6)=193, a(7)=351.
   The function memoises previously computed terms so that
   calls may arrive in any order.   */

#include <stdlib.h>

/* Multiply by a small unsigned integer (coeff ≤ 5) using repeated addition.
   Works for any unsigned seq_t type. */
static seq_t mul_small(seq_t x, unsigned int coeff)
{
    seq_t res = 0;
    for (unsigned int i = 0; i < coeff; ++i)
        res += x;
    return res;
}

/* Cached terms, 1‑indexed (cache[0] unused).  All elements are zeroed
   on allocation to allow a simple test for initialization. */
static seq_t *cache = NULL;
static size_t cache_size = 0;

/* Ensure the cache can hold indices up to 'n' (inclusive). */
static void ensure_cache(idx_t n)
{
    size_t required = (size_t)n + 1;          /* 1‑indexed plus sentinel */
    if (required < 8) required = 8;           /* at least hold the first 7 terms */

    if (cache_size >= required)
        return;

    size_t new_size = cache_size ? cache_size * 2 : 8;
    if (new_size < required) new_size = required;

    seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
    if (!tmp) abort();
    /* Zero the new part if the array grew. */
    if (new_size > cache_size)
        for (size_t i = cache_size; i < new_size; ++i)
            tmp[i] = 0;
    cache = tmp;
    cache_size = new_size;
}

/* Initialise the known small terms once.  If they are already set,
   this does nothing. */
static void init_bases(void)
{
    if (cache[1] != 0) return;    /* cache already initialised */
    cache[0] = 0;                 /* unused sentinel */
    cache[1] = (seq_t)1;
    cache[2] = (seq_t)7;
    cache[3] = (seq_t)21;
    cache[4] = (seq_t)49;
    cache[5] = (seq_t)101;
    cache[6] = (seq_t)193;
    cache[7] = (seq_t)351;
}

/* Main sequence function.  Returns a(n) for n ≥ 1; a(0) returns 0. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;

    size_t idx = (size_t)n;
    ensure_cache(idx);
    init_bases();

    /* If the term is already computed, return it. */
    if (cache[idx] != 0)
        return cache[idx];

    /* Compute terms from 8 up to idx.  Missing intermediate terms are
       computed on the fly; memoisation keeps them for future calls. */
    for (size_t i = 8; i <= idx; ++i) {
        seq_t t1 = cache[i - 1];
        seq_t t2 = cache[i - 2];
        seq_t t3 = cache[i - 3];
        seq_t t4 = cache[i - 4];
        seq_t t5 = cache[i - 5];

        /* sum = 4*t1 + t3 + 2*t4  (all additions are safe in unsigned)
           sub = 5*t2 + t5            (also safe) */
        seq_t sum = mul_small(t1, 4) + t3 + mul_small(t4, 2);
        seq_t sub = mul_small(t2, 5) + t5;
        seq_t res = sum - sub;          /* sum >= sub for all n ≥ 8 */

        cache[i] = res;
    }
    return cache[idx];
}
