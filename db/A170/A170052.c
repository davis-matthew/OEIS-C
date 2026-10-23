/*
 * A170052  Number of reduced words of length n in Coxeter group on 43
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^36 = I.
 * https://oeis.org/A170052
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 11, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A170052 (reduced words in the Coxeter group on 43 generators)
   Recurrence derived from the generating function:
   For n = 0: a(0) = 1
   For 1 ≤ n ≤ 35: a(n) = 41 * (a(n-1)+…+a(0)) + 2
   For 36 ≤ n:   a(n) = 41 * (a(n-1)+…+a(n-35)) – 861 * a(n-36)
   (The constants come from the numerator and denominator of the rational
    generating function given in the OEIS entry.)
   The function is memoised so that repeated calls with arbitrary n
   are handled in O(1) per new term.                                                    */

#include <stdlib.h>

seq_t a(idx_t n) {
    /* static cache for memoised values */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;          /* number of computed terms, cache[0..cache_len-1] */

    /* enlarge cache if necessary */
    if (n >= cache_len) {
        idx_t new_len = n + 1;
        seq_t *new_cache = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        if (!new_cache) abort();            /* out‑of‑memory; abort is acceptable here */
        /* compute missing terms */
        for (idx_t i = cache_len; i <= n; ++i) {
            /* compute sum of the previous at most 35 terms */
            idx_t m = (i < 35) ? i : 35;     /* number of terms to include */
            seq_t sum = 0;
            for (idx_t k = 1; k <= m; ++k)
                sum += new_cache[i - k];
            /* start with 41 * sum */
            seq_t ai = sum * 41;
            /* subtract 861 * a(i-36) for i >= 36 */
            if (i >= 36)
                ai -= new_cache[i - 36] * 861;
            /* add the numerator coefficient b_i */
            seq_t b = 0;
            if (i == 0 || i == 36)
                b = 1;
            else if (1 <= i && i <= 35)
                b = 2;
            /* else b remains 0 */
            ai += b;
            new_cache[i] = ai;
        }
        cache = new_cache;
        cache_len = new_len;
    }
    return cache[(size_t)n];
}
