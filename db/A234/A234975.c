/*
 * A234975  Number of (n+1) X (1+1) 0..4 arrays with every 2 X 2 subblock
 *          having the sum of the absolute values of the edge differences
 *          equal to 8.
 * https://oeis.org/A234975
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 9*a(n-1) + 13*a(n-2) - 134*a(n-3) + a(n-4) + 480*a(n-5) -
 *            224*a(n-6) - 96*a(n-7) initial terms: a(1)=182, a(2)=1664,
 *            a(3)=14420, a(4)=127982, a(5)=1127550, a(6)=9962574,
 *            a(7)=87941706 (indices start at 1 as in OEIS)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 8, u64 n <= 18, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* 
 * seq_t and idx_t are defined by the build system.
 * This function may be called in any order; it memoises results.
 */
seq_t a(idx_t n)
{
    /* Static memoisation pool */
    static seq_t *cache = NULL;
    static idx_t cached_n = 0;    /* number of terms already computed (>=0) */

    /* Helper: initialise the first seven terms when needed */
    if (cache == NULL) {
        /* Allocate up to at least 7 terms, or more if n is larger */
        idx_t alloc_sz = (n > 7) ? n : 7;
        cache = (seq_t *)malloc(alloc_sz * sizeof(seq_t));
        if (!cache) exit(1);

        /* first seven terms */
        cache[0] = (seq_t)182;          /* a(1) */
        cache[1] = (seq_t)1664;         /* a(2) */
        cache[2] = (seq_t)14420;        /* a(3) */
        cache[3] = (seq_t)127982;       /* a(4) */
        cache[4] = (seq_t)1127550;      /* a(5) */
        cache[5] = (seq_t)9962574;      /* a(6) */
        cache[6] = (seq_t)87941706;     /* a(7) */

        cached_n = 7;
    }

    /* Expand memoisation pool if required */
    if (n > cached_n) {
        idx_t old_sz = cached_n;
        idx_t new_sz = n;
        seq_t *tmp = (seq_t *)realloc(cache, new_sz * sizeof(seq_t));
        if (!tmp) exit(1);
        cache = tmp;

        /* Precompute missing terms using the recurrence */
        for (idx_t k = old_sz + 1; k <= new_sz; ++k) {
            /* Index in array is k-1 */
            seq_t t1 = cache[k - 2];          /* a(k-1) */
            seq_t t2 = cache[k - 3];          /* a(k-2) */
            seq_t t3 = cache[k - 4];          /* a(k-3) */
            seq_t t4 = cache[k - 5];          /* a(k-4) */
            seq_t t5 = cache[k - 6];          /* a(k-5) */
            seq_t t6 = cache[k - 7];          /* a(k-6) */
            seq_t t7 = cache[k - 8];          /* a(k-7) */

            cache[k - 1] = (seq_t)9   * t1
                         + (seq_t)13  * t2
                         - (seq_t)134 * t3
                         + (seq_t)1   * t4
                         + (seq_t)480 * t5
                         - (seq_t)224 * t6
                         - (seq_t)96  * t7;
        }
        cached_n = new_sz;
    }

    /* n >= 1, now safely return the value */
    return cache[n - 1];
}
