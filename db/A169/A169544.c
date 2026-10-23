/*
 * A169544  Number of reduced words of length n in Coxeter group on 3
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^35 = I.
 * https://oeis.org/A169544
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 31, u64 n <= 31, u128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A169544 – Number of reduced words in the Coxeter group on 3 generators
 *  with (S_i)^2 = (S_i S_j)^35 = I.
 *
 *  The ordinary generating function is
 *      (1 + t + … + t^34) / (1 - 2t + t^2 - 2t^3 + ... + t^34 - 2t^35 + t^36).
 *
 *  From this we obtain the linear recurrence
 *
 *      a(0) = 1,
 *      for n ≥ 1:
 *
 *          let m = min(n, 34)
 *          let S1 = Σ_{k even, 2≤k≤m} a(n-k)
 *          let S2 = Σ_{k odd , 1≤k≤m} a(n-k)
 *
 *          a(n) = 2·S2 – S1 + (n ≤ 34 ? 1 : 0)
 *
 *  This recurrence uses only additions and subtractions of unsigned
 *  integers, so it works for any seq_t (including arbitrary‑precision
 *  integer classes).
 *
 *  The function remembers previously computed values, so any order of
 *  calls is handled correctly.
 *
 *  Parameters
 *      n : idx_t   – the index (offset 0).
 *
 *  Returns
 *      seq_t   – the nth term of the sequence.
 */
#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static cache for computed terms. */
    static seq_t *cache   = NULL;   /* array of terms */
    static size_t  cap    = 0;      /* allocated size */
    static idx_t   max    = 0;      /* highest index computed */

    /* Ensure storage up to n. */
    if (cache == NULL) {
        cap = (size_t)(n + 1);
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!cache) return (seq_t)0;      /* out of memory – abort silently */
        cache[0] = (seq_t)1;              /* a(0) = 1 */
        max      = 0;
    } else if ((size_t)(n + 1) > cap) {
        size_t new_cap = (size_t)(n + 1);
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return (seq_t)0;
        cache = tmp;
        cap   = new_cap;
    }

    /* Compute any missing terms up to n. */
    for (idx_t i = max + 1; i <= n; ++i) {
        idx_t limit = (i <= 34 ? i : 34);   /* number of previous terms used */

        seq_t S1 = 0;   /* sum over even k */
        seq_t S2 = 0;   /* sum over odd  k */

        for (idx_t k = 1; k <= limit; ++k) {
            if (k & 1)      /* odd */
                S2 += cache[i - k];
            else            /* even */
                S1 += cache[i - k];
        }

        /* term = 2*S2 – S1 (+1 if i ≤ 34). */
        seq_t term = S2 + S2;      /* 2 * S2 */
        term -= S1;
        if (i <= 34)
            term += (seq_t)1;

        cache[i] = term;
    }

    max = n;
    return cache[n];
}
