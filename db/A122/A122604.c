/*
 * A122604  Expansion of x*(x^3+3*x^2-1)*(5*x^4-5*x^2+1) / (
 *          -1+x+8*x^2-7*x^3-21*x^4+15*x^5+20*x^6-10*x^7-5*x^8+x^9 ).
 * https://oeis.org/A122604
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, less
 * Formula:   not stated
 * Range:     u8 n <= 15, u16 n <= 22, u32 n <= 38, u64 n <= 38, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A122604
 * The sequence satisfies the linear recurrence
 *   a(n) =  a(n‑1) + 8a(n‑2) – 7a(n‑3) – 21a(n‑4)
 *          +15a(n‑5) +20a(n‑6) –10a(n‑7) –5a(n‑8) + a(n‑9)   for n ≥ 10
 * with initial values a(1) = … = a(9) = 1.
 *
 * The implementation uses memoisation so calls may appear in any order.
 */

#include <stdlib.h>
#include <string.h>

static const int REC[10] = {
    /* d0 = -1 */ -1,
    /* d1 */      1,
    /* d2 */      8,
    /* d3 */     -7,
    /* d4 */    -21,
    /* d5 */     15,
    /* d6 */     20,
    /* d7 */    -10,
    /* d8 */     -5,
    /* d9 */      1
};

seq_t a(idx_t n)
{
    /* static cache: cache[0] is unused, cache[1]..cache[n] holds the terms */
    static seq_t *cache = NULL;
    static idx_t cached_n = 0;          /* largest index currently stored */

    /* Expand cache if necessary */
    if (n > cached_n) {
        idx_t new_n = n;
        cache = (seq_t *)realloc(cache, (new_n + 1) * sizeof(seq_t));
        if (!cache) return 0;          /* allocation failure – return 0 */

        /* initialise any new cells with zero (realloc does not clear) */
        memset(cache + cached_n + 1, 0,
               (new_n - cached_n) * sizeof(seq_t));

        /* fill in required terms */
        for (idx_t i = cached_n + 1; i <= new_n; ++i) {
            if (i <= 9) {
                cache[i] = 1;          /* a(1)..a(9) = 1 */
            } else {
                /* recurrence: a(i) = Σ_{k=1}^{9} d_k * a(i-k)  */
                seq_t pos = 0;
                seq_t neg = 0;
                for (int k = 1; k <= 9; ++k) {
                    seq_t prod = cache[i - k] *
                                 (seq_t)(REC[k] > 0 ? REC[k] : -REC[k]);
                    if (REC[k] > 0)
                        pos += prod;
                    else
                        neg += prod;
                }
                cache[i] = pos - neg;   /* pos >= neg for this sequence */
            }
        }
        cached_n = new_n;
    }

    return cache[n];
}
