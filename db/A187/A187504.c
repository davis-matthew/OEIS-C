/*
 * A187504  Let i be in {1,2,3,4} and let r >= 0 be an integer. Let p = {p_1,
 *          p_2, p_3, p_4} = {-1,0,1,2}, n=3*r+p_i, and define a(-1)=0. Then
 *          a(n)=a(3*r+p_i) gives the quantity of H_(9,2,0) tiles in a
 *          subdivided H_(9,i,r) tile after linear scaling by the factor Q^r,
 *          where Q=sqrt(x^3-2*x) with x=2*cos(Pi/9).
 * https://oeis.org/A187504
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*a(n-3) + 3*a(n-6) – a(n-9) – a(n-12), for n ≥ 12, with
 *            a(0)=1,a(1)=0,a(2)=0,a(3)=0,a(4)=1,a(5)=1,a(6)=2,a(7)=2,a(8)=2,a(9)=4,a(10)=6,a(11)=7
 * Range:     u8 n <= 20, u16 n <= 36, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Initial 12 values of A187504. */
static const seq_t init_vals[12] = {
    (seq_t)1, (seq_t)0, (seq_t)0, (seq_t)0,
    (seq_t)1, (seq_t)1, (seq_t)2, (seq_t)2,
    (seq_t)2, (seq_t)4, (seq_t)6, (seq_t)7
};

/* Cached sequence values. */
static seq_t *cache = NULL;
/* Capacity (total allocated slots). */
static idx_t cache_cap = 0;
/* Number of values that have already been computed and stored in cache. */
static idx_t cache_len = 0;

/* Ensure that cache has space for indices up to n. */
static void ensure_cache(idx_t n)
{
    if (n + 1 <= cache_cap)
        return;

    size_t new_cap = cache_cap ? cache_cap : 1;
    while (new_cap <= n)
        new_cap <<= 1;                 /* double until sufficient */

    seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (new_cache == NULL)
        abort();                       /* allocation failed */

    cache = new_cache;
    cache_cap = new_cap;
}

/* Return the n‑th term of the OEIS sequence A187504. */
seq_t a(idx_t n)
{
    /* For the first 12 indices we can answer directly from the constant table. */
    if (n < 12)
        return init_vals[n];

    /* Make sure we have enough storage. */
    ensure_cache(n);

    /* Compute any values that have not yet been generated. */
    for (idx_t i = cache_len; i <= n; ++i) {
        if (i < 12)                     /* should not happen for i > 11, but safety */
            cache[i] = init_vals[i];
        else
            cache[i] = 2 * cache[i - 3]
                     + 3 * cache[i - 6]
                     - cache[i - 9]
                     - cache[i - 12];
    }

    /* Mark all values up to n as computed. */
    cache_len = n + 1;

    return cache[n];
}
