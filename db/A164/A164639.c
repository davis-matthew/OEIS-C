/*
 * A164639  Number of reduced words of length n in Coxeter group on 26
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^7 = I.
 * https://oeis.org/A164639
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) satisfies a rational generating function a(t) = (t^7 + 2t^6
 *            + 2t^5 + 2t^4 + 2t^3 + 2t^2 + 2t + 1) / (1 - 24t - 24t^2 - 24t^3
 *            - 24t^4 - 24t^5 - 24t^6 + 300t^7). From the denominator we
 *            obtain the linear recurrence for n ≥ 8: a(n) = 24 a(n−1) + 24
 *            a(n−2) + 24 a(n−3) + 24 a(n−4) + 24 a(n−5) + 2...
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 13, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* Memoisation array */
    static seq_t *cache = NULL;
    static size_t  cap   = 0;     /* allocated elements (indices 0 … cap-1) */

    /* Initialise cache with the first eight values. */
    if (cap == 0) {
        cap = 8;                     /* indices 0 … 7 */
        cache = (seq_t*)malloc(cap * sizeof(seq_t));
        if (!cache) return (seq_t)0; /* allocation failure – return 0 */

        cache[0] = (seq_t)1;
        cache[1] = (seq_t)26;
        cache[2] = (seq_t)650;
        cache[3] = (seq_t)16250;
        cache[4] = (seq_t)406250;
        cache[5] = (seq_t)10156250;
        cache[6] = (seq_t)253906250;

        /* a(7) comes from the inhomogeneous part of the recurrence. */
        cache[7] =   (seq_t)24 * cache[6]
                  + (seq_t)24 * cache[5]
                  + (seq_t)24 * cache[4]
                  + (seq_t)24 * cache[3]
                  + (seq_t)24 * cache[2]
                  + (seq_t)24 * cache[1]
                  - (seq_t)300 * cache[0]
                  + (seq_t)1;
    }

    /* The caller may request any index n.  Handle indices below the
       pre‑computed range by simply returning the stored value. */
    if ((size_t)n < cap)
        return cache[(size_t)n];

    /* Grow the cache to accommodate index n. */
    size_t new_cap = ((size_t)n) + 1;
    seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) return (seq_t)0;  /* allocation failure – return 0 */

    cache = tmp;
    size_t old_cap = cap;
    cap = new_cap;

    /* Compute missing terms with the homogeneous recurrence. */
    for (size_t i = old_cap; i <= (size_t)n; ++i) {
        /* For i <= 7 we already have the values. */
        if (i <= 7) continue;  /* safety; should not occur */
        cache[i] =   (seq_t)24 * cache[i - 1]
                   + (seq_t)24 * cache[i - 2]
                   + (seq_t)24 * cache[i - 3]
                   + (seq_t)24 * cache[i - 4]
                   + (seq_t)24 * cache[i - 5]
                   + (seq_t)24 * cache[i - 6]
                   - (seq_t)300 * cache[i - 7];
    }

    return cache[(size_t)n];
}
