/*
 * A209347  Number of 6-bead necklaces labeled with numbers -n..n allowing
 *          reversal, with sum zero with no three beads in a row equal.
 * https://oeis.org/A209347
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 4*a(n-1) - 5*a(n-2) + a(n-3) + a(n-4) + a(n-5) + a(n-6)
 *            -5*a(n-7) + 4*a(n-8) - a(n-9) Initial values: a(1) = 14 , a(2) =
 *            146 , a(3) = 770 , a(4) = 2698 , a(5) = 7358 , a(6) = 16968 ,
 *            a(7) = 34720 , a(8) = 64942 , a(9) = 113288
 * Range:     u8 n <= 2, u16 n <= 8, u32 n <= 77, u64 n <= 210, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    /* static cache */
    static seq_t *cache = NULL;
    static idx_t len = 0;      /* number of terms currently computed */
    static idx_t cap = 0;      /* allocated capacity */

    /* initialize cache with first 9 terms on first call */
    if (cache == NULL) {
        /* allocate small initial capacity, then grow as needed */
        cap = 16;
        cache = (seq_t*)malloc(cap * sizeof(seq_t));
        if (!cache) return 0; /* out of memory */
        /* set first nine terms */
        cache[0] = (seq_t)14;
        cache[1] = (seq_t)146;
        cache[2] = (seq_t)770;
        cache[3] = (seq_t)2698;
        cache[4] = (seq_t)7358;
        cache[5] = (seq_t)16968;
        cache[6] = (seq_t)34720;
        cache[7] = (seq_t)64942;
        cache[8] = (seq_t)113288;
        len = 9;
    }

    if (n <= len)
        return cache[n-1];

    /* grow capacity if needed */
    if (n > cap) {
        idx_t new_cap = cap ? cap : 1;
        while (new_cap < n)
            new_cap *= 2;
        seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp)
            return 0; /* out of memory */
        cache = tmp;
        cap = new_cap;
    }

    /* compute terms up to n */
    for (idx_t k = len + 1; k <= n; ++k) {
        idx_t idx = k - 1; /* zero‑based index of term a(k) */

        /* compute using positive and negative parts to avoid
           unsigned underflow: pos >= neg for all n by construction */
        seq_t pos = 4 * cache[idx-1]   /* 4*a(k-1)   */
                 + cache[idx-3]        /*   +a(k-3)  */
                 + cache[idx-4]        /*   +a(k-4)  */
                 + cache[idx-5]        /*   +a(k-5)  */
                 + cache[idx-6]        /*   +a(k-6)  */
                 + 4 * cache[idx-8];   /*   +4*a(k-8) */

        seq_t neg = 5 * cache[idx-2]   /* 5*a(k-2)    */
                 + 5 * cache[idx-7]   /* +5*a(k-7)   */
                 + cache[idx-9];       /* +a(k-9)     */

        cache[idx] = pos - neg;
    }
    len = n;
    return cache[n-1];
}
