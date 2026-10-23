/*
 * A170456  Number of reduced words of length n in Coxeter group on 15
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^45 = I.
 * https://oeis.org/A170456
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = p_n + 13*∑_{i=0}^{n-1} a(i) for 0 ≤ n < 45, where p_0 = 1
 *            and p_k = 2 for 1 ≤ k < 45; a(n) = 13*∑_{i=1}^{44} a(n-i) –
 *            91*a(n-45) for n ≥ 45. Initial values: a(0)=1, a(1)=15,
 *            a(2)=210, … (see OEIS A170456)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 16, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;      /* stores computed terms a(0)…a(k) */
    static size_t cap   = 0;         /* allocated capacity */
    static size_t size  = 0;         /* number of terms computed */

    static seq_t sumAll   = 0;       /* Σ a(i) for i < current size (i.e. up to size-1) */
    static seq_t sumLast44 = 0;      /* Σ a(currentIndex - i) for i=1..44 when size≥45 */

    /* Grow the cache if necessary */
    if (n + 1 > cap) {
        size_t newcap = cap ? cap * 2 : 16;
        while (newcap < (size_t)(n + 1))
            newcap *= 2;
        seq_t *newptr = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
        if (!newptr) abort();           /* allocation failed */
        cache = newptr;
        cap = newcap;
    }

    /* If the requested term is already known, return it */
    if (n < size)
        return cache[n];

    /* Compute terms up to n (inclusive) */
    for (idx_t i = size; i <= n; ++i) {
        seq_t val;
        if (i < 45) {                     /* early terms */
            seq_t pn = (i == 0) ? 1 : 2;   /* p_i from the numerator of the g.f. */
            val = pn + (seq_t)13 * sumAll;
            sumAll += val;                 /* update cumulative sum for next step */
        } else {                          /* recurrence for i ≥ 45 */
            if (i == 45)
                sumLast44 = sumAll - cache[0];   /* Σ a(1)…a(44) */
            val = (seq_t)13 * sumLast44 - (seq_t)91 * cache[i - 45];
            sumLast44 = sumLast44 + val - cache[i - 44]; /* update sliding sum */
            sumAll += val;                 /* keep sumAll consistent (not used later) */
        }
        cache[i] = val;
    }

    size = n + 1;
    return cache[n];
}
