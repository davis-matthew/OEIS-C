/*
 * A063215  Dimension of the space of weight 2n cuspidal newforms for Gamma_0(
 *          47 ).
 * https://oeis.org/A063215
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(1) = 4, a(2) = 11, a(3) = 19, a(4) = 27, a(5) = 35, a(6) = 41
 *            a(n) = a(n‑2) + a(n‑3) – a(n‑5) for n > 6 (Sequence A063215 –
 *            dimension of the space of weight 2n cuspidal newforms for
 *            Γ₀(47)) This implementation memoises computed terms and expands
 *            the cache as needed
 * Range:     u8 n <= 33, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Return 0 for n == 0 (out of bounds); otherwise return the requested term. */
    if (n == 0) return 0;

    /* Static cache for previously computed terms. */
    static seq_t *cache      = NULL;  /* array of computed values (0‑based index => a(i+1)) */
    static idx_t computed_len = 0;    /* number of values already computed (max n) */
    static idx_t allocated_len = 0;   /* current allocated capacity */

    /* Return cached result if already available. */
    if (n <= computed_len) {
        return cache[n-1];
    }

    /* Ensure the array is large enough. */
    if (n > allocated_len) {
        seq_t *tmp = (seq_t *)realloc(cache, n * sizeof(seq_t));
        if (!tmp) abort();      /* memory allocation failed */
        cache = tmp;
        allocated_len = n;
    }

    /* Compute new terms from the first missing index up to n. */
    for (idx_t i = computed_len + 1; i <= n; ++i) {
        seq_t val;
        if (i == 1) val = 4;
        else if (i == 2) val = 11;
        else if (i == 3) val = 19;
        else if (i == 4) val = 27;
        else if (i == 5) val = 35;
        else if (i == 6) val = 41;
        else {
            /* a(i) = a(i-2) + a(i-3) - a(i-5) */
            val = cache[i-3] + cache[i-4] - cache[i-6];
        }
        cache[i-1] = val;
    }

    computed_len = n;
    return cache[n-1];
}
