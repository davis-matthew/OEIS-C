/*
 * A094094  Define x[1]...x[n] by the equations Sum_{j=1..n} x[j]^i = i,
 *          i=1..n; a(n) = n! * Sum_{j=1..n} x[j]^(n+1).
 * https://oeis.org/A094094
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (3n-1)a(n-1) - n(3n-4)a(n-2) + (n-2)(n-1)n a(n-3) with
 *            a(1)=1, a(2)=5, a(3)=25. This follows from the given recurrence
 *            for sequence A094094. The implementation caches all values
 *            computed so far in a static array up to a modest maximum, so
 *            repeated calls in arbitrary order stay O(n) overall
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 11, u64 n <= 19, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>   /* for size_t, if needed */

static seq_t cache[20001] = {0};   /* enough for n up to ~20000 */
static idx_t cache_len = 0;        /* largest n cached (0 if none) */

seq_t a(idx_t n)
{
    if (n == 0) {                /* undefined in OEIS, but return 0 */
        return (seq_t)0;
    }

    /* Initialise base cases once */
    if (cache_len == 0) {
        cache_len = 3;
        cache[1] = (seq_t)1;   /* a(1) */
        cache[2] = (seq_t)5;   /* a(2) */
        cache[3] = (seq_t)25;  /* a(3) */
    }

    /* If the requested n is already cached, return it */
    if (n <= cache_len) {
        return cache[n];
    }

    /* Compute terms up to n */
    for (idx_t i = cache_len + 1; i <= n; ++i) {
        /* Term (3i-1)*a(i-1) */
        seq_t t1 = (seq_t)(3 * i - 1) * cache[i - 1];

        /* Term - i*(3i-4)*a(i-2) */
        seq_t t3 = (seq_t)i * (seq_t)(3 * i - 4) * cache[i - 2];

        /* Term + (i-2)(i-1)i * a(i-3) */
        seq_t t2 = (seq_t)(i - 2) * (seq_t)(i - 1) * (seq_t)i * cache[i - 3];

        cache[i] = t1 - t3 + t2;
    }

    cache_len = n;
    return cache[n];
}
