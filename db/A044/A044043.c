/*
 * A044043  Number of tilings of 2 X n rectangle with polyominoes, each of
 *          which has area = # of adjacent polyominoes.
 * https://oeis.org/A044043
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3*a(n-3)+2*a(n-4)+a(n-5)+3*a(n-6)+4*a(n-7)+2*a(n-10) * +
 *            c(n) * where c(n) is a correction term derived from the
 *            numerator of the * generating function: * c(3)= 2, c(4)= 5,
 *            c(5)= 3, c(6)= 3, c(7)= 4, c(8)= 4, c(11)= 2, * c(n)=0
 *            otherwise. * a(1)=0, a(2)=0, a(3)=2, a(4)=5, a(5)=3, a(6)=9,
 *            a...
 * Range:     u8 n <= 12, u16 n <= 21, u32 n <= 36, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Helpers:   correction, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/seq_realloc.h"
#include "../utils/correction.h"

/* Initial known values a(1)..a(10).  Index 0 is unused. */
static const seq_t init_vals[11] = {
    (seq_t)0,  /* unused */
    (seq_t)0,  /* a(1) */
    (seq_t)0,  /* a(2) */
    (seq_t)2,  /* a(3) */
    (seq_t)5,  /* a(4) */
    (seq_t)3,  /* a(5) */
    (seq_t)9,  /* a(6) */
    (seq_t)23, /* a(7) */
    (seq_t)25, /* a(8) */
    (seq_t)44, /* a(9) */
    (seq_t)113 /* a(10) */
};

/* Cache of computed terms.  cache[0] is set to 0 for convenience. */
static seq_t *cache = NULL;
static size_t cache_size = 0;          /* current allocated size (>=1) */

/* Compute a(n) with memoisation. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;     /* no term at n=0, but safe */

    /* Expand cache if necessary */
    if ((size_t)n >= cache_size) {
        size_t new_size = (size_t)n + 1;          /* allow index n */
        seq_t *tmp = seq_realloc(cache, cache_size, new_size);
        if (!tmp) return (seq_t)0;   /* allocation failure */

        cache = tmp;
        if (cache_size == 0) cache[0] = (seq_t)0;

        /* Compute missing entries */
        for (idx_t i = (idx_t)cache_size; i <= n; ++i) {
            if (i <= 10) {
                cache[i] = init_vals[i];
            } else {
                seq_t sum;
                sum = cache[i-3] * (seq_t)3
                     + cache[i-4] * (seq_t)2
                     + cache[i-5]
                     + cache[i-6] * (seq_t)3
                     + cache[i-7] * (seq_t)4
                     + cache[i-10] * (seq_t)2;
                sum += correction(i);
                cache[i] = sum;
            }
        }
        cache_size = new_size;
    }
    return cache[n];
}
