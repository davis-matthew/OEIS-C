/*
 * A143009  Crystal ball sequence for the A3 x A3 lattice.
 * https://oeis.org/A143009
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (10*n^6+30*n^5+85*n^4+120*n^3+121*n^2+66*n+18)/18
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 43, u64 n <= 1792,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Crystal ball sequence for the A3 x A3 lattice.  This implementation
   uses constant–time finite–difference updates to avoid any intermediate
   overflow.  The sequence is stored in a static cache; subsequent calls
   reuse previously computed terms.  The updates rely only on
   addition/subtraction of already computed values, so the method works
   for any integer type `seq_t`, including 64‑bit unsigned integers and
   arbitrary‑precision integer classes. */

#include <stdlib.h>
#include <stdbool.h>

/* static cache of computed terms */
static seq_t *cache = NULL;
static size_t cache_size = 0;   /* number of terms currently cached */
static size_t cache_cap  = 0;   /* allocated capacity of cache */

/* store the last six forward differences for the current highest index */
static seq_t diff[7];          /* diff[0] is a(n), diff[1] is Δa(n),
                                  diff[2] is Δ^2 a(n), …, diff[6] is Δ^6 a(n) */

/* initialize cache with first 7 terms and compute initial differences */
static void init_cache(void)
{
    /* allocate initial capacity */
    cache_cap = 16;
    cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));

    /* first 7 terms of the sequence (a(0) … a(6)) */
    cache[0] = (seq_t)1;
    cache[1] = (seq_t)25;
    cache[2] = (seq_t)253;
    cache[3] = (seq_t)1445;
    cache[4] = (seq_t)5741;
    cache[5] = (seq_t)17861;
    cache[6] = (seq_t)46705;
    cache_size = 7;

    /* compute finite differences up to order 6 */
    seq_t t0[7], t1[6], t2[5], t3[4], t4[3], t5[2];
    for (int i = 0; i < 7; i++) t0[i] = cache[i];
    for (int i = 0; i < 6; i++) t1[i] = t0[i+1] - t0[i];
    for (int i = 0; i < 5; i++) t2[i] = t1[i+1] - t1[i];
    for (int i = 0; i < 4; i++) t3[i] = t2[i+1] - t2[i];
    for (int i = 0; i < 3; i++) t4[i] = t3[i+1] - t3[i];
    for (int i = 0; i < 2; i++) t5[i] = t4[i+1] - t4[i];
    seq_t t6 = t5[1] - t5[0];

    diff[0] = cache[6];      /* a(6) */
    diff[1] = t1[5];         /* Δa(6) = a(6)-a(5)      */
    diff[2] = t2[4];         /* Δ²a(6)                 */
    diff[3] = t3[3];         /* Δ³a(6)                 */
    diff[4] = t4[2];         /* Δ⁴a(6)                 */
    diff[5] = t5[1];         /* Δ⁵a(6)                 */
    diff[6] = t6;            /* Δ⁶a(6) = constant      */
}

/* grow cache capacity when needed */
static void grow_cache(void)
{
    if (cache_size >= cache_cap) {
        cache_cap *= 2;
        cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
    }
}

/* main function requested by the problem statement */
seq_t a(idx_t n)
{
    static bool initialized = false;
    if (!initialized) {
        init_cache();
        initialized = true;
    }

    /* if the requested term is already cached, return it */
    if (n < cache_size)
        return cache[n];

    /* grow cache up to the required index, computing new terms */
    while (cache_size <= n) {
        /* update differences from order 5 down to 1 */
        for (int k = 5; k >= 1; --k)
            diff[k] += diff[k+1];
        /* delta of order 0 (the term itself) */
        diff[0] += diff[1];

        /* store new term in cache */
        grow_cache();
        cache[cache_size] = diff[0];
        ++cache_size;
    }

    return cache[n];
}
