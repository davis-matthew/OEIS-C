/*
 * A221922  Number of nX2 arrays of occupancy after each element stays put or
 *          moves to some king-move neighbor, without move-in move-out
 *          straight through or left turns.
 * https://oeis.org/A221922
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 19, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A221922
 *  Recurrence: a(n) = 9*a(n-1) +12*a(n-2) -29*a(n-3) +41*a(n-4) +75*a(n-5)
 *                -129*a(n-6) -30*a(n-7) +61*a(n-8) -a(n-9) -4*a(n-10)
 *  Initial values:
 *    a(1)  = 3
 *    a(2)  = 35
 *    a(3)  = 350
 *    a(4)  = 3487
 *    a(5)  = 34714
 *    a(6)  = 345744
 *    a(7)  = 3443699
 *    a(8)  = 34300167
 *    a(9)  = 341638088
 *    a(10) = 3402798388
 *  The function computes values using the recurrence and memoises
 *  the sequence in a static cache, so calls may be made in any order.
 */

#include <stddef.h>
#include <stdlib.h>

static const seq_t init_vals[10] = {
    (seq_t)3,            /* a(1) */
    (seq_t)35,           /* a(2) */
    (seq_t)350,          /* a(3) */
    (seq_t)3487,         /* a(4) */
    (seq_t)34714,        /* a(5) */
    (seq_t)345744,       /* a(6) */
    (seq_t)3443699,      /* a(7) */
    (seq_t)34300167,     /* a(8) */
    (seq_t)341638088,    /* a(9) */
    (seq_t)3402798388ULL/* a(10) */
};

static seq_t *cache   = NULL;   /* cached terms */
static idx_t cache_len = 0;     /* number of computed terms (1‑based) */
static size_t cache_cap = 0;    /* allocated capacity */

/* Ensure cache can hold at least n terms (1‑based). */
static void ensure_cache(idx_t n)
{
    if (cache_cap >= (size_t)n)
        return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 16;
    while (new_cap < (size_t)n)
        new_cap <<= 1;
    seq_t *new_arr = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!new_arr) abort();           /* abort on exhaustion */
    cache = new_arr;
    cache_cap = new_cap;
}

/* Compute all terms up to index n (1‑based). */
static void compute_up_to(idx_t n)
{
    if (n <= cache_len)
        return;  /* already available */

    ensure_cache(n);

    for (idx_t i = cache_len; i < n; ++i) {  /* i is zero‑based index for a(i+1) */
        seq_t val;
        if (i < 10) {
            val = init_vals[i];
        } else {
            /* a(n) depends on a(n-1)...a(n-10) */
            val = ((seq_t)9)  * cache[i-1]  +
                  ((seq_t)12) * cache[i-2]  -
                  ((seq_t)29) * cache[i-3]  +
                  ((seq_t)41) * cache[i-4]  +
                  ((seq_t)75) * cache[i-5]  -
                  ((seq_t)129)* cache[i-6]  -
                  ((seq_t)30) * cache[i-7]  +
                  ((seq_t)61) * cache[i-8]  -
                  cache[i-9] -
                  ((seq_t)4)  * cache[i-10];
        }
        cache[i] = val;
    }
    cache_len = n;
}

/* Public interface: compute a(n) for n ≥ 1. */
seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;  /* undefined for n < 1 */

    compute_up_to(n);
    return cache[n-1];
}
