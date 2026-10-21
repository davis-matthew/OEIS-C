/*
 * A080806  Positive integer values of n such that 6*n^2-5 is a square.
 * https://oeis.org/A080806
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 10*a(n-2) - a(n-4) (n ≥ 5) with a(1)=1, a(2)=3, a(3)=7,
 *            a(4)=29. The function returns the nth term of the sequence
 *            A080806. The implementation uses memoisation so that calls with
 *            arbitrary ordering of n are handled efficiently
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 20, u64 n <= 39, u128 n <= 78,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* memoisation table indexed from 1 */
    static idx_t computed = 0;      /* highest index computed so far */
    static seq_t *cache   = NULL;   /* array of terms, cache[0] unused */
    static idx_t cap      = 0;      /* allocated size */

    if (n == 0)                     /* no term 0, return 0 */
        return 0;

    /* grow the table if necessary */
    if (cap <= n) {
        void *tmp = realloc(cache, (n + 1) * sizeof(seq_t));
        if (!tmp)                    /* out‑of‑memory; return 0 as a safe fallback */
            return 0;
        cache = (seq_t *)tmp;
        cap = n + 1;
    }

    /* compute missing terms in order */
    for (idx_t i = computed + 1; i <= n; ++i) {
        if (i == 1)           cache[i] = 1;
        else if (i == 2)      cache[i] = 3;
        else if (i == 3)      cache[i] = 7;
        else if (i == 4)      cache[i] = 29;
        else                  cache[i] = 10 * cache[i - 2] - cache[i - 4];
    }
    computed = n;

    return cache[n];
}
