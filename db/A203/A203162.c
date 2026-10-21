/*
 * A203162  (n-1)-st elementary symmetric function of the first n terms of
 *          (1,2,3,1,2,3,1,2,3,...).
 * https://oeis.org/A203162
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 12*a(n-3) - 36*a(n-6), a(1)=1, a(2)=3, a(3)=11, a(4)=17,
 *            a(5)=40, a(6)=132
 * Range:     u8 n <= 7, u16 n <= 14, u32 n <= 32, u64 n <= 68, u128 n <= 142,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the (n-1)-st elementary symmetric function of the first n terms of
   the repeating sequence (1,2,3,1,2,3,...) */

#include <stdint.h>

/* Maximum number of terms we will cache.  The sequence is tested for n up to ~10000. */
#define CACHE_MAX 10000

static seq_t cache[CACHE_MAX];   /* cached values, 0‑based (cache[0] == a(1)) */
static idx_t cache_size = 0;     /* number of cached entries */

/* Returns a(n). */
seq_t a(idx_t n)
{
    /* Basic cases can be answered immediately. */
    if (n == 0) return 0;              /* off‑offset, unused */
    if (n >= 1 && n <= 6) {
        static const seq_t first6[6] = { (seq_t)1,  (seq_t)3,  (seq_t)11,
                                         (seq_t)17, (seq_t)40, (seq_t)132 };
        return first6[n-1];
    }

    /* Ensure the cache is large enough and compute missing terms. */
    if (cache_size == 0) {
        /* Fill the first six entries.  cache[0] .. cache[5] correspond to a(1)..a(6). */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)3;
        cache[2] = (seq_t)11;
        cache[3] = (seq_t)17;
        cache[4] = (seq_t)40;
        cache[5] = (seq_t)132;
        cache_size = 6;
    }

    while (cache_size < n) {
        /* Compute the next term using the 6‑term linear recurrence. */
        seq_t term = (cache[cache_size-3] * (seq_t)12) -
                     (cache[cache_size-6] * (seq_t)36);
        cache[cache_size] = term;
        cache_size++;
    }

    return cache[n-1];
}
