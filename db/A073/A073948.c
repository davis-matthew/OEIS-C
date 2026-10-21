/*
 * A073948  Number of strings over Z_3 of length n with trace 0 and subtrace
 *          1.
 * https://oeis.org/A073948
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 12, u32 n <= 22, u64 n <= 29, u128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence over trace t∈{0,1,2} and subtrace s∈{0,1,2}
   a(n; t, s) = a(n-1; t, s) + a(n-1; t+2, s+2t+1) + a(n-1; t+1, s+t+1)
   with a(0;0,0)=1, others 0.
   We return a(n) = a(n; 0, 1).                                                                  */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache of already computed terms, 0‑indexed: cache[k] = a(k+1) */
    static seq_t *cache = NULL;
    static idx_t cached = 0;  /* number of terms cached so far */
    /* DP state for the current n: DP[t*3+s] = a(current_n; t, s) */
    static seq_t prev[9];
    static int initialized = 0;

    /* initialize DP for n = 0 */
    if (!initialized) {
        for (int i = 0; i < 9; ++i) prev[i] = 0;
        prev[0] = 1;                    /* a(0; 0, 0) = 1 */
        initialized = 1;
    }

    if (n == 0) return 0; /* undefined, but safe */

    /* if we already have the value, just return it */
    if (n <= cached) return cache[(size_t)(n - 1)];

    /* grow the cache to hold up to n terms */
    seq_t *tmp = (seq_t *)realloc(cache, (size_t)n * sizeof(seq_t));
    if (!tmp) abort();
    cache = tmp;

    /* compute forward from the greatest cached n + 1 up to the requested n */
    for (idx_t i = cached + 1; i <= n; ++i)
    {
        seq_t curr[9] = { 0 };  /* zero the new DP row */

        /* propagate all transitions for a new symbol a ∈ {0,1,2} */
        for (int t = 0; t < 3; ++t)
        {
            for (int s = 0; s < 3; ++s)
            {
                seq_t val = prev[t * 3 + s];
                if (val == 0) continue;
                for (int a = 0; a < 3; ++a)
                {
                    int nt = (t + a) % 3;
                    int ns = (s + a * t) % 3;
                    curr[nt * 3 + ns] += val;
                }
            }
        }

        /* copy new DP row into prev for the next iteration */
        for (int j = 0; j < 9; ++j) prev[j] = curr[j];

        /* store a(i) = curr[0*3 + 1] (trace 0, subtrace 1) */
        cache[(size_t)(i - 1)] = curr[1];
    }

    cached = n;
    return cache[(size_t)(n - 1)];
}
