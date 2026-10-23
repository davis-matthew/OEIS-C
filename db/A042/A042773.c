/*
 * A042773  Denominators of continued fraction convergents to sqrt(917).
 * https://oeis.org/A042773
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 16, u64 n <= 34, u128 n <= 69,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Denominators of continued fraction convergents to sqrt(917).
   Recurrence:
     Q_0 = 1,   Q_{-1} = 0
     Q_k = a_k * Q_{k-1} + Q_{k-2}   (k ≥ 1)
   where a_k are the partial quotients of sqrt(917), with period 16:
     a_1..a_16 = [3,1,1,4,1,14,3,8,3,14,1,4,1,1,3,60]  (a_16 = 2*a0).
   The function returns the n‑th denominator (a(0)=1). */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* period of partial quotients for sqrt(917) */
    static const seq_t PART[] = {3,1,1,4,1,14,3,8,3,14,1,4,1,1,3,60};
    static const idx_t P = sizeof(PART)/sizeof(PART[0]);

    /* static cache of computed denominators */
    static seq_t *cache = NULL;
    static size_t cap = 0;
    static idx_t last = 0;          /* highest index computed so far */

    /* initialise cache on first call */
    if (cache == NULL) {
        cap = 8;
        cache = (seq_t*)malloc(cap * sizeof(seq_t));
        cache[0] = (seq_t)1;        /* Q_0 = 1 */
        last  = 0;
    }

    /* extend cache up to |n| if necessary */
    if (n > last) {
        /* ensure capacity */
        if (cache == NULL) return 0; /* safety, should never happen */
        if (cap <= (size_t)n) {
            size_t new_cap = cap ? cap << 1 : 8;
            while (new_cap <= (size_t)n) new_cap <<= 1;
            seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) return 0;                 /* allocation failed */
            cache = tmp;
            cap = new_cap;
        }

        /* initialise previous two terms for the recurrence */
        seq_t q_prev2 = (last >= 1) ? cache[last-1] : (seq_t)0; /* Q_{k-2} */
        seq_t q_prev1 = cache[last];                          /* Q_{k-1} */

        /* compute Q_k for k = last+1 .. n */
        for (idx_t k = last + 1; k <= n; ++k) {
            seq_t a_k = PART[(k - 1) % P];
            seq_t q_k = a_k * q_prev1 + q_prev2;
            cache[k] = q_k;
            q_prev2 = q_prev1;
            q_prev1 = q_k;
        }
        last = n;
    }
    return cache[n];
}
