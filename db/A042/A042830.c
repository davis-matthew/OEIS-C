/*
 * A042830  Numerators of continued fraction convergents to sqrt(946).
 * https://oeis.org/A042830
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   a(n) = numerator of the n-th convergent to sqrt(946). Continued
 *            fraction of sqrt(946): a0 = 30, period L = 30 with partial
 *            quotients:
 *            [1,3,8,1,1,6,3,3,1,3,1,1,1,2,30,2,1,1,1,3,1,3,3,6,1,1,8,3,1,60].
 *            Recurrence: p_0 = 30 p_{-1} = 1 p_k = a_k * p_{k-1} + p_{k-2} (k
 *            ≥ 1) The sequence a(n) is p_n
 * Range:     u8 n <= 2, u16 n <= 7, u32 n <= 19, u64 n <= 39, u128 n <= 82,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static const unsigned int a_period[30] = {
    1, 3, 8, 1, 1, 6, 3, 3, 1, 3,
    1, 1, 1, 2, 30, 2, 1, 1, 1, 3,
    1, 3, 3, 6, 1, 1, 8, 3, 1, 60
};
#define PERIOD_LEN 30

seq_t a(idx_t n) {
    static seq_t *cache = NULL;
    static size_t cache_len = 0;
    static seq_t p_prev1 = 0, p_prev2 = 0;   /* p_{k-1} and p_{k-2} */

    /* Initialisation on first call */
    if (cache_len == 0) {
        cache_len = 1;
        cache = (seq_t*)malloc(cache_len * sizeof(seq_t));
        if (!cache) { /* allocation failed – aborting is safe */
            exit(EXIT_FAILURE);
        }
        cache[0] = (seq_t)30;   /* p_0 */
        p_prev1 = (seq_t)30;    /* p_{k-1} for k=1 */
        p_prev2 = (seq_t)1;     /* p_{-1} */
    }

    /* Compute terms on-demand up to index n */
    if ((size_t)n >= cache_len) {
        size_t new_len = (size_t)n + 1;
        seq_t *tmp = (seq_t*)realloc(cache, new_len * sizeof(seq_t));
        if (!tmp) exit(EXIT_FAILURE);  /* allocation failure */
        cache = tmp;

        for (idx_t k = cache_len; k <= n; ++k) {
            unsigned int ak = a_period[(k - 1) % PERIOD_LEN];
            seq_t cur = ((seq_t)ak) * p_prev1 + p_prev2;
            cache[(size_t)k] = cur;
            p_prev2 = p_prev1;
            p_prev1 = cur;
        }
        cache_len = new_len;
    }

    return cache[(size_t)n];
}
