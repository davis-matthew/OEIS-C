/*
 * A208545  Number of 7-bead necklaces of n colors allowing reversal, with no
 *          adjacent beads having the same color.
 * https://oeis.org/A208545
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 8*a(n-1) - 28*a(n-2) + 56*a(n-3) - 70*a(n-4) + 56*a(n-5)
 *            - 28*a(n-6) + 8*a(n-7) - a(n-8) (n>8) Initial values: a(1)=0,
 *            a(2)=0, a(3)=9, a(4)=156, a(5)=1170, a(6)=5580, a(7)=19995,
 *            a(8)=58824
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 35, u64 n <= 210, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    if (n == 0) return 0;          /* safety for non‑positive index */

    static seq_t *cache = NULL;    /* 0‑based array holding a(1)..a(k) */
    static idx_t computed = 0;     /* highest n for which cache is valid */

    /* initialise cache with the first eight terms */
    if (!cache) {
        cache = (seq_t*)calloc(8, sizeof(seq_t));
        if (!cache) return 0;      /* allocation failed */
        cache[0] = 0;              /* a(1) */
        cache[1] = 0;              /* a(2) */
        cache[2] = 9;              /* a(3) */
        cache[3] = 156;            /* a(4) */
        cache[4] = 1170;           /* a(5) */
        cache[5] = 5580;           /* a(6) */
        cache[6] = 19995;          /* a(7) */
        cache[7] = 58824;          /* a(8) */
        computed = 8;
    }

    /* grow cache if needed */
    if (n > computed) {
        seq_t *newcache = (seq_t*)realloc(cache, n * sizeof(seq_t));
        if (!newcache) return 0;   /* allocation failed */
        cache = newcache;

        /* compute missing values using the recurrence */
        for (idx_t i = computed + 1; i <= n; ++i) {
            /* i > 8 guarantees that the recurrence applies */
            seq_t val = (seq_t)8  * cache[i-2];
            val -= (seq_t)28 * cache[i-3];
            val += (seq_t)56 * cache[i-4];
            val -= (seq_t)70 * cache[i-5];
            val += (seq_t)56 * cache[i-6];
            val -= (seq_t)28 * cache[i-7];
            val += (seq_t)8  * cache[i-8];
            val -=       cache[i-9];
            cache[i-1] = val;
        }
        computed = n;
    }

    /* return the requested term (1‑based index) */
    return cache[n-1];
}
