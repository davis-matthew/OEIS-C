/*
 * A192778  Coefficient of x in the reduction of the n-th Fibonacci polynomial
 *          by x^3->x^2+3x+1.
 * https://oeis.org/A192778
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 13, u32 n <= 24, u64 n <= 46, u128 n <= 89,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A192778 – Coefficient of x in the reduction of the n‑th Fibonacci
    polynomial by the rule x^3 → x^2 + 3x + 1.

    Recurrence (for n ≥ 7):
        a(n) = a(n‑1) + 6*a(n‑2) – a(n‑3) – 6*a(n‑4) + a(n‑5) + a(n‑6).

    Initial values (for 1 ≤ n ≤ 6):
        a(1)=0, a(2)=1, a(3)=0, a(4)=5, a(5)=4, a(6)=28.
*/

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static cache of previously computed terms.
       - cache[0] is unused (keeps indices 1…n).
       - cache[1]…cache[6] hold the initial values.
    */
    static seq_t *cache   = NULL;    /* allocated array of seq_t */
    static idx_t  cap     = 0;       /* number of indices that can be stored (including 0) */
    static idx_t  last    = 0;       /* largest index already computed */

    /* ------------------------------------------------------------------ */
    /* Initialization of the cache on the first call.                    */
    /* ------------------------------------------------------------------ */
    if (cache == NULL) {
        /* Allocate space for indices 0 … 6 – later we will double capacity. */
        cap = 7;
        /* Use a cast that is acceptable in both C and C++.                */
#ifdef __cplusplus
        cache = reinterpret_cast<seq_t*>(malloc(cap * sizeof(seq_t)));
#else
        cache = (seq_t*)malloc(cap * sizeof(seq_t));
#endif
        /* Store the initial values.  cache[0] is unused. */
        cache[0] = (seq_t)0;
        cache[1] = (seq_t)0;
        cache[2] = (seq_t)1;
        cache[3] = (seq_t)0;
        cache[4] = (seq_t)5;
        cache[5] = (seq_t)4;
        cache[6] = (seq_t)28;
        last = 6;
    }

    /* ------------------------------------------------------------------ */
    /* If n is one of the already stored base cases, return it directly. */
    /* ------------------------------------------------------------------ */
    if (n <= last) return cache[n];

    /* ------------------------------------------------------------------ */
    /* Ensure the buffer is large enough for indices up to n.             */
    /* ------------------------------------------------------------------ */
    if (n >= cap) {
        idx_t new_cap = cap;
        while (new_cap <= n) new_cap <<= 1;
#ifdef __cplusplus
        seq_t *tmp = reinterpret_cast<seq_t*>(realloc(cache,
                                                       new_cap * sizeof(seq_t)));
#else
        seq_t *tmp = (seq_t*)realloc(cache,
                                     new_cap * sizeof(seq_t));
#endif
        cache = tmp;
        cap   = new_cap;
    }

    /* ------------------------------------------------------------------ */
    /* Compute all missing terms up to n using the recurrence.           */
    /* ------------------------------------------------------------------ */
    for (idx_t i = last + 1; i <= n; ++i) {
        /* recurrence: a(i) = a(i-1)+6*a(i-2)-a(i-3)-6*a(i-4)+a(i-5)+a(i-6) */
        seq_t t = cache[i-1];
        t += cache[i-2] * (seq_t)6;
        t -= cache[i-3];
        t -= cache[i-4] * (seq_t)6;
        t += cache[i-5];
        t += cache[i-6];
        cache[i] = t;
    }
    last = n;
    return cache[n];
}
