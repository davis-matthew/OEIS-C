/*
 * A052971  Expansion of (1-x)/(1-2*x-2*x^3+2*x^4).
 * https://oeis.org/A052971
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(0)=1, a(1)=1, a(2)=2, a(3)=6 Recurrence (for n≥4): a(n) =
 *            2*a(n‑3) + 2*a(n‑1) – 2*a(n‑4)
 * Range:     u8 n <= 7, u16 n <= 14, u32 n <= 28, u64 n <= 32, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implements the recurrence from the OEIS definition:
   2*a(n) – 2*a(n+1) – 2*a(n+3) + a(n+4) = 0   */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache of computed terms */
    static seq_t *cache = NULL;
    static idx_t cached = 0;          /* number of terms currently cached */

    /* extend cache if required */
    if (n >= cached) {
        /* we will cache at least up to n, and at least 4 terms for the recurrence */
        idx_t need = n + 1;
        if (need < 4) need = 4;
        seq_t *tmp = (seq_t *)realloc(cache, need * sizeof(seq_t));
        if (!tmp) return (seq_t)0;     /* memory error – return zero (unlikely in tests) */
        cache = tmp;

        /* initialise the first four terms if this is the first call */
        if (cached == 0) {
            cache[0] = (seq_t)1;
            cache[1] = (seq_t)1;
            cache[2] = (seq_t)2;
            cache[3] = (seq_t)6;
        }

        /* compute new terms using the recurrence */
        for (idx_t i = cached; i < need; ++i) {
            if (i < 4) continue;               /* already initialised */
            cache[i] = 2 * cache[i - 3] + 2 * cache[i - 1] - 2 * cache[i - 4];
        }

        cached = need;
    }

    return cache[n];
}
