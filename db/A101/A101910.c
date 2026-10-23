/*
 * A101910  A fractal sequence such that a(n) = A101911(A000120(n-1)) for n>0,
 *          where A101911 is the binomial transform of this sequence.
 * https://oeis.org/A101910
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  eigen, nonn
 * Formula:   a(0)=1; for n>0, a(n) = Σ_{k=0}^{n-1} Mod(C(n-1,k),2) *
 *            a(A000120(n-k-1)) (Lucas) (where Mod(C,2)=1 iff C(n-1,k) is odd)
 * Range:     u8 n <= 78, u16 n <= 78, u32 n <= 78, u64 n <= 78, u128 n <= 78,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Memoisation cache */
    static seq_t *cache = NULL;
    static idx_t capacity = 0;   /* allocated array size   */
    static idx_t up_to = 0;      /* highest index already computed */

    /* Initialise cache on first call */
    if (cache == NULL) {
        capacity = 1;
        cache = (seq_t *)malloc(capacity * sizeof(seq_t));
        if (!cache) abort();
        cache[0] = (seq_t)1;   /* a(0) = 1 */
        up_to   = 0;
    }

    /* Grow cache to accommodate all indices up to n */
    if (n > up_to) {
        /* Expand capacity as needed */
        while (capacity <= n)
            capacity <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, capacity * sizeof(seq_t));
        if (!tmp) abort();
        cache = tmp;

        /* Compute missing values */
        for (idx_t i = up_to + 1; i <= n; ++i) {
            uint64_t m = (uint64_t)i - 1;        /* n-1 as 64-bit integer */

            /* Sum over all k such that C(m,k) is odd.
               By Lucas theorem, this is equivalent to all submasks of m. */
            seq_t sum = (seq_t)0;
            uint64_t sub = m;
            for (;;) {
                uint64_t t = m - sub;                 /* n-1-k */
                uint64_t pc = __builtin_popcountll(t);/* A000120(t) */
                sum += cache[(idx_t)pc];

                if (sub == 0) break;
                sub = (sub - 1) & m;                  /* next submask */
            }
            cache[(idx_t)i] = sum;
        }
        up_to = n;
    }

    return cache[(idx_t)n];
}
