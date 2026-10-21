/*
 * A231338  Number of (n+1) X (2+1) 0..3 arrays with no element unequal to a
 *          strict majority of its horizontal and vertical neighbors, with
 *          values 0..3 introduced in row major order.
 * https://oeis.org/A231338
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) follows the linear recurrence * a(n) = 5*a(n-1) - 6*a(n-2)
 *            + 2*a(n-3) - 8*a(n-4) * + 6*a(n-5) - a(n-6) + 3*a(n-7) * with
 *            initial values * a(1)=4, a(2)=9, a(3)=22, a(4)=59, a(5)=159, *
 *            a(6)=439, a(7)=1236. * The sequence starts at offset 1: a(1) is
 *            the first term
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 20, u64 n <= 41, u128 n <= 81,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* a(0) is undefined for the OEIS sequence, return 0 if requested. */
    if (n == 0) return (seq_t)0;

    /* Static cache of computed terms, 1‑based index. */
    static seq_t *cache = NULL;   /* cache[0] unused, cache[1] holds a(1) */
    static size_t cap   = 0;      /* number of allocated slots (including index 0) */
    static idx_t  max_computed = 0;   /* greatest n for which cache[n] is valid */

    /* Ensure we have space for at least index n and for the first 7 terms. */
    size_t needed = (size_t)n + 1;
    if (needed < 8) needed = 8;            /* space for a(1)..a(7) */
    if (cap < needed) {
        seq_t *tmp = (seq_t *)realloc(cache, needed * sizeof(seq_t));
        if (!tmp && needed > 0) {          /* allocation failed – exit safely */
            return (seq_t)0;
        }
        cache = tmp;
        cap   = needed;
    }

    /* Initialize the first seven terms once. */
    if (max_computed == 0) {
        cache[0] = (seq_t)0;
        cache[1] = (seq_t)4;
        cache[2] = (seq_t)9;
        cache[3] = (seq_t)22;
        cache[4] = (seq_t)59;
        cache[5] = (seq_t)159;
        cache[6] = (seq_t)439;
        cache[7] = (seq_t)1236;
        max_computed = 7;
    }

    /* If the requested term is already computed, return it. */
    if (n <= max_computed) {
        return cache[n];
    }

    /* Compute all terms up to n using the recurrence. */
    for (idx_t i = max_computed + 1; i <= n; ++i) {
        seq_t res = (seq_t)5 * cache[i - 1];
        res -= (seq_t)6 * cache[i - 2];
        res += (seq_t)2 * cache[i - 3];
        res -= (seq_t)8 * cache[i - 4];
        res += (seq_t)6 * cache[i - 5];
        res -=  cache[i - 6];
        res += (seq_t)3 * cache[i - 7];
        cache[i] = res;
    }

    max_computed = n;
    return cache[n];
}
