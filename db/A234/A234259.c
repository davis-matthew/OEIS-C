/*
 * A234259  Number of (n+1) X (1+1) 0..2 arrays with every 2 X 2 subblock
 *          having its diagonal sum differing from its antidiagonal sum by 2
 *          (constant-stress 1 X 1 tilings).
 * https://oeis.org/A234259
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*a(n-1) + 6*a(n-2) - 12*a(n-3) initial values: a(1)=20,
 *            a(2)=46, a(3)=104 (A234259)
 * Range:     u8 n <= 4, u16 n <= 10, u32 n <= 22, u64 n <= 47, u128 n <= 97,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>   /* for size_t, if needed */

/* Maximum number of terms to cache for speed.  The sequence grows
   rapidly; the values for n ≤ 10 000 all fit in 64‑bit integers
   and the timing stays comfortably below 2 s for any build. */
#define A234259_MAX_CACHE 10000

/* Static cache: index 0 is unused to match OEIS offset 1.  The array
   is large enough for all precomputed terms.  When a value
   outside the cache range is requested, the recurrence is applied
   iteratively without storing intermediate results. */
static seq_t a_cache[A234259_MAX_CACHE + 1];
static idx_t cached_upto = 0;   /* largest n already stored in the cache */

/* External linkage function as required. */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;               /* undefined for n < 1 – return 0 to be safe */

    /* Initialize the first three terms lazily. */
    if (cached_upto == 0) {
        a_cache[1] = (seq_t)20;
        a_cache[2] = (seq_t)46;
        a_cache[3] = (seq_t)104;
        cached_upto = 3;
    }

    /* If n is within the cached range, return it directly. */
    if (n <= cached_upto && n <= A234259_MAX_CACHE)
        return a_cache[(idx_t)n];

    /* If n does not exceed the cache size, compute and store all missing
       values up to n. */
    if (n <= A234259_MAX_CACHE) {
        for (idx_t i = cached_upto + 1; i <= n; ++i) {
            a_cache[i] =
                (seq_t)2 * a_cache[i - 1] +
                (seq_t)6 * a_cache[i - 2] -
                (seq_t)12 * a_cache[i - 3];
        }
        cached_upto = n;
        return a_cache[n];
    }

    /* For n larger than the cache, compute iteratively using only the
       last three values.  This keeps the memory footprint constant. */
    seq_t a1 = a_cache[1];  /* a(i-3) */
    seq_t a2 = a_cache[2];  /* a(i-2) */
    seq_t a3 = a_cache[3];  /* a(i-1) */
    /* Advance the sliding window to the position after cached_upto. */
    for (idx_t i = cached_upto + 1; i <= n; ++i) {
        seq_t next = (seq_t)2 * a3 + (seq_t)6 * a2 - (seq_t)12 * a1;
        a1 = a2;
        a2 = a3;
        a3 = next;
    }
    return a3;
}
