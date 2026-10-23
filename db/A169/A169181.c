/*
 * A169181  Number of reduced words of length n in Coxeter group on 24
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^27 = I.
 * https://oeis.org/A169181
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 14, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence generating A169181.  
 * a(0) = 1
 * For 1 ≤ n ≤ 26: a(n) = 2 + 22 * Σ_{k=1}^{n} a(n-k)
 * For n = 27:       a(27) = 1 + 22 * Σ_{k=1}^{26} a(27-k) - 253 * a(0)
 * For n ≥ 28:       a(n) = 22 * Σ_{k=1}^{26} a(n-k) - 253 * a(n-27)
 *
 * The implementation memoizes computed terms in a static cache and
 * extends it as needed.  The recurrence uses only seq_t arithmetic
 * (addition, subtraction, multiplication by small constants) and
 * works for any integer type chosen for seq_t, including arbitrary‑precision.
 */

#include <stdlib.h>   /* for realloc, size_t */

static seq_t *cache = NULL;   /* memoization buffer */
static size_t cache_len = 0;  /* number of elements currently stored */

/* Return a(n) for the Coxeter group reduced word count. */
seq_t a(idx_t n) {
    /* Grow cache if required. */
    if (!(cache_len && n < cache_len)) {
        size_t new_len = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        if (!tmp) return 0;  /* allocation failed – return zero (unlikely). */
        cache = tmp;

        /* Compute terms from current length up to n (inclusive). */
        for (idx_t i = cache_len; i <= n; ++i) {
            seq_t sum = 0;

            if (i <= 26) {
                /* Sum of the previous i terms (when i≤26). */
                for (idx_t k = 1; k <= i; ++k) sum += cache[i - k];
                seq_t coeff = (i == 0) ? 1 : 2;
                cache[i] = coeff + 22 * sum;
            } else {
                /* Sum of the previous 26 terms (i≥27). */
                for (idx_t k = 1; k <= 26; ++k) sum += cache[i - k];
                seq_t coeff = (i == 27) ? 1 : 0;
                cache[i] = coeff + 22 * sum - 253 * cache[i - 27];
            }
        }
        cache_len = new_len;
    }
    return cache[n];
}
