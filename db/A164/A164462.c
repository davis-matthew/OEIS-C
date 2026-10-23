/*
 * A164462  Number of binary strings of length n with no substrings equal to
 *          0001 0100 or 0101.
 * https://oeis.org/A164462
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 21, u32 n <= 44, u64 n <= 90, u128 n <= 183,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A164462 – number of length‑n binary strings avoiding
   the substrings 0001 0100 and 0101.
   Recurrence for n ≥ 9:
   a(n) = 2·a(n‑1) – a(n‑2) + a(n‑3) – a(n‑5),
   with initial values
   a(4)=13, a(5)=20, a(6)=33, a(7)=55, a(8)=89. */
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* For n < 4 the definition is not needed – return 0. */
    if (n < 4)
        return (seq_t)0;

    /* Static cache – once initialised it retains values for all
       subsequent calls. */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;      /* allocated length */

    size_t idx = (size_t)n;           /* index into cache */

    /* Initialise cache with the known starting values. */
    if (!cache) {
        cache_len = 9;                /* indices 0 … 8 */
        cache = (seq_t *)malloc(cache_len * sizeof(seq_t));
        if (!cache)
            return (seq_t)0;          /* allocation failure – give up */

        /* Unused entries for indices < 4. */
        cache[0] = cache[1] = cache[2] = cache[3] = (seq_t)0;
        cache[4] = (seq_t)13;
        cache[5] = (seq_t)20;
        cache[6] = (seq_t)33;
        cache[7] = (seq_t)55;
        cache[8] = (seq_t)89;
    }

    /* Grow cache if necessary and compute missing terms. */
    if (idx >= cache_len) {
        size_t new_len = idx + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        if (!tmp)
            return cache[cache_len - 1];   /* cannot grow – return last value */
        cache = tmp;

        for (size_t i = cache_len; i <= idx; ++i) {
            /* Recurrence applies for i ≥ 9; for i < 9 the value
               has already been initialised. */
            seq_t t = cache[i - 1];
            t = t + t;            /* 2·a(i‑1) */
            t = t - cache[i - 2]; /* – a(i‑2) */
            t = t + cache[i - 3]; /* + a(i‑3) */
            t = t - cache[i - 5]; /* – a(i‑5) */
            cache[i] = t;
        }
        cache_len = new_len;
    }

    return cache[idx];
}
