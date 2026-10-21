/*
 * A210003  Number of binary words of length n containing no subword 10001.
 * https://oeis.org/A210003
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 33, u64 n <= 67, u128 n <= 134,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * OEIS A210003: Number of binary words of length n containing no subword
 * 10001.
 *
 * Recurrence:
 *   a(0) = 1, a(1) = 2, a(2) = 4, a(3) = 8, a(4) = 16
 *   a(n) = 2*a(n-1) - a(n-4) + a(n-5)  (n >= 5)
 *
 * The function memoises previous values so that repeated calls are fast
 * and may be made in any order.
 */

#include <stdlib.h>            /* malloc, realloc */

/* Compute the n-th term of OEIS sequence A210003. */
seq_t a(idx_t n)
{
    static seq_t *cache     = NULL;   /* memoisation buffer */
    static idx_t max_computed = (idx_t)-1;  /* sentinel: nothing computed yet */

    /* If the needed index is beyond what we have, grow the cache. */
    if (max_computed == (idx_t)-1 || n > max_computed) {
        size_t new_size = (size_t)(n + 1) * sizeof(seq_t);
        seq_t *tmp = (seq_t *)realloc(cache, new_size);
        if (!tmp) return (seq_t)0;      /* allocation failure – undefined result */

        cache = tmp;

        /* Compute from the first missing index up to n. */
        idx_t start = (max_computed == (idx_t)-1) ? 0 : max_computed + 1;
        for (idx_t i = start; i <= n; ++i) {
            if (i < 5) {
                /* a(i) = 2^i */
                cache[i] = ((seq_t)1) << i;
            } else {
                seq_t t = cache[i-1] + cache[i-1];  /* 2*a(i-1) */
                t -= cache[i-4];                    /* -a(i-4)   */
                t += cache[i-5];                    /* +a(i-5)   */
                cache[i] = t;
            }
        }
        max_computed = n;
    }

    return cache[n];
}
