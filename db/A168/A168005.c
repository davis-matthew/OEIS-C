/*
 * A168005  Numbers k with property that first digit of 9*k = last digit of k.
 * https://oeis.org/A168005
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 741, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n) as the n‑th positive integer k satisfying
   first digit of 9*k equals last digit of k.  Brute‑force
   search with memoization.  */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache to store found terms */
    static seq_t *cache = NULL;
    static idx_t   cap   = 0;   /* allocated capacity of cache   */
    static idx_t   size  = 0;   /* number of stored terms        */
    static idx_t   cur   = 0;   /* last tested k (0 means none)  */

    /* initialise cache if first call */
    if (cache == NULL) {
        cap   = 1;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
    }

    /* find terms until we have a(n) */
    while (size < n) {
        cur++;  /* next candidate k */

        /* product 9*k; may overflow for small seq_t types,
           but values that overflow will never appear in the
           sequence that fits in seq_t, so it is acceptable   */
        seq_t prod = (seq_t)9 * (seq_t)cur;

        /* most significant digit of prod */
        seq_t msd = prod;
        while (msd >= (seq_t)10) msd /= (seq_t)10;

        /* check condition */
        if (msd == (seq_t)(cur % (idx_t)10)) {
            /* grow cache if needed */
            if (size == cap) {
                cap *= 2;
                cache = (seq_t *)realloc(cache, cap * sizeof(seq_t));
            }
            cache[size++] = cur;
        }
    }

    return cache[n - 1];  /* n is 1‑based */
}
