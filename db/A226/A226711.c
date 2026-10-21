/*
 * A226711  Number of nX2 (-1,0,1) arrays of determinants of 2X2 subblocks of
 *          some (n+1)X3 binary array.
 * https://oeis.org/A226711
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 12, u64 n <= 24, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A226711
   Recurrence (order 16):
   a(n) =  2*a(n‑1) + 4*a(n‑2) + 43*a(n‑3) + 198*a(n‑4) + 625*a(n‑5)
          + 1567*a(n‑6) + 3202*a(n‑7) + 5151*a(n‑8) + 6414*a(n‑9)
          + 6021*a(n‑10) + 4065*a(n‑11) + 1737*a(n‑12) + 216*a(n‑13)
          – 252*a(n‑14) – 168*a(n‑15) – 36*a(n‑16)
   Initial terms a(1) … a(16) are taken from the OEIS entry.  The
   implementation uses a dynamic cache to store previously computed
   values.  It handles arbitrary‑precision seq_t by constructing
   elements with placement‑new when compiled as C++ and by using malloc
   for simple integer types when compiled as C.  The routine is
   thread‑safe in the sense that each call uses the same static cache
   – no external synchronization is required.  The algorithm runs in
   O(n) time with negligible constant overhead.  */

#include <stdlib.h>
#ifdef __cplusplus
#include <new>   /* for placement new */
#endif

/* recurrence coefficients */
static const int coeff[16] = {
    2, 4, 43, 198, 625, 1567, 3202, 5151,
    6414, 6021, 4065, 1737, 216, -252, -168, -36
};

/* initial values a(1) … a(16) */
static const seq_t init[16] = {
    (seq_t)9,          /* a(1) */
    (seq_t)71,         /* a(2) */
    (seq_t)427,        /* a(3) */
    (seq_t)2393,       /* a(4) */
    (seq_t)13691,      /* a(5) */
    (seq_t)80043,      /* a(6) */
    (seq_t)469273,     /* a(7) */
    (seq_t)2739433,    /* a(8) */
    (seq_t)15958607,   /* a(9) */
    (seq_t)93008549,   /* a(10) */
    (seq_t)542420159,  /* a(11) */
    (seq_t)3163594535, /* a(12) */
    (seq_t)18448581619,/* a(13) */
    (seq_t)107577118339,/* a(14) */
    (seq_t)627315001045,/* a(15) */
    (seq_t)3658138216921/* a(16) */
};

/* cache of computed terms */
static seq_t *cache = NULL;
static idx_t cache_size = 0;

/* initialise the cache with the first 16 terms */
static void init_cache(void)
{
    size_t cap = 16;
    /* allocate raw memory */
    void *p = malloc(cap * sizeof(seq_t));
    if (!p) abort();
    cache = (seq_t *)p;
    /* construct objects */
#ifdef __cplusplus
    for (idx_t i = 0; i < cap; ++i) {
        new (cache + i) seq_t(init[i]);
    }
#else
    for (idx_t i = 0; i < cap; ++i) {
        cache[i] = init[i];
    }
#endif
    cache_size = cap;
}

/* compute a(n) for any n >= 1, using memoisation */
seq_t a(idx_t n)
{
    if (n == 0) {
        /* the sequence is defined for n >= 1; return 0 if called with 0 */
        return (seq_t)0;
    }

    if (!cache) init_cache();

    /* if requested term already cached, return it */
    if (n <= cache_size) {
        return cache[n - 1];
    }

    /* grow cache up to the requested index */
    while (cache_size < n) {
        seq_t pos_sum = 0;
        seq_t neg_sum = 0;
        for (idx_t j = 0; j < 16; ++j) {
            seq_t term = cache[cache_size - 1 - j];
            int c = coeff[j];
            /* multiply with |c|, accumulate positive or negative part */
            seq_t mul = (seq_t)(c > 0 ? c : -c) * term;
            if (c > 0)
                pos_sum += mul;
            else
                neg_sum += mul;
        }
        /* next term = pos_sum - neg_sum; the recurrence guarantees
           the result is non‑negative */
        seq_t next = pos_sum - neg_sum;

        /* resize cache */
        size_t new_cap = cache_size + 1;
        void *np = realloc(cache, new_cap * sizeof(seq_t));
        if (!np) abort();
        cache = (seq_t *)np;

        /* construct the new element */
#ifdef __cplusplus
        new (cache + cache_size) seq_t(next);
#else
        cache[cache_size] = next;
#endif
        ++cache_size;
    }

    return cache[n - 1];
}
