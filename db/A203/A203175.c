/*
 * A203175  Number of nX2 0..2 arrays with every 1 immediately preceded by 0
 *          to the left or above, no 0 immediately preceded by a 0, and every
 *          2 immediately preceded by 0 1 to the left or above.
 * https://oeis.org/A203175
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + 2*a(n-3) Initial values: a(1)=1, a(2)=1, a(3)=2
 *            Computes the n‑th term for n >= 1 with memoisation
 * Range:     u8 n <= 12, u16 n <= 22, u32 n <= 43, u64 n <= 85, u128 n <= 169,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

/* External types */
extern seq_t a(idx_t n);

seq_t a(idx_t n)
{
    /* Memoisation buffer: indices start at 1 (cache[0] unused) */
    static seq_t *cache = NULL;
    static size_t   cap   = 0; /* allocated capacity, in elements (incl. 0) */
    static idx_t    size  = 0; /* highest index computed so far, zero means none */

    /* Sequence defined only for n >= 1 */
    if (n < 1) {
        return (seq_t)0;
    }

    /* Ensure capacity for indices [0 .. n] */
    if ((size_t)n + 1 > cap) {
        size_t new_cap = (size_t)n + 1;
        /* Grow in blocks of 128 to limit reallocation cost */
        if (new_cap < 128) {
            new_cap = 128;
        } else if (new_cap < cap * 2) {
            new_cap = cap * 2;
        }
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) {
            /* Allocation failure – return zero term for safety */
            return (seq_t)0;
        }
        cache = tmp;
        cap   = new_cap;
    }

    /* If the requested value is already known, return it */
    if (n <= size) {
        return cache[n];
    }

    /* Compute successive terms up to n */
    for (idx_t i = size + 1; i <= n; ++i) {
        if (i == 1) {
            cache[i] = (seq_t)1;
        } else if (i == 2) {
            cache[i] = (seq_t)1;
        } else if (i == 3) {
            cache[i] = (seq_t)2;
        } else {
            /* a(i) = a(i-1) + 2*a(i-3) */
            seq_t tmp = cache[i - 3];
            tmp = tmp + tmp;           /* multiply by 2 */
            cache[i] = cache[i - 1] + tmp;
        }
    }

    size = n;
    return cache[n];
}
