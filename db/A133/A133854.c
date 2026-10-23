/*
 * A133854  a(n)*a(n-13) = a(n-1)*a(n-12)+a(n-6)+a(n-7) with initial terms
 *          a(1)=...=a(13)=1.
 * https://oeis.org/A133854
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n)*a(n-13) = a(n-1)*a(n-12)+a(n-6)+a(n-7), a(1)=...=a(13)=1 *
 *            Linear recurrence: a(n) = 124*(a(n-12)-a(n-24))+a(n-36) for n≥37
 *            * All intermediate calculations use only integer +, -, /
 *            operations. * For n ≤ 36 the quadratic recurrence is used to
 *            produce the initial values
 * Range:     u8 n <= 25, u16 n <= 33, u32 n <= 68, u64 n <= 122,
 *            u128 n <= 232, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Positive indices only; behaviour for n<1 is unspecified. */
    if (n == 0)
        return (seq_t)0;        /* safeguard, never used in OEIS queries */

    /* Static cache: 1‑indexed array of already computed values */
    static seq_t *cache = NULL;
    static idx_t cap   = 0;      /* largest index stored so far */

    /* Lazy allocation up to request size */
    if (cap == 0) {
        /* we need at least the first 13 terms */
        idx_t init = n > 13 ? n : 13;
        cache = (seq_t *)malloc((init + 1) * sizeof(seq_t));
        if (!cache)
            exit(EXIT_FAILURE);
        for (idx_t i = 1; i <= 13; ++i)
            cache[i] = (seq_t)1;
        cap = 13;
    }

    /* Grow cache if needed, computing missing terms on the fly */
    if (n > cap) {
        idx_t new_cap = n;
        seq_t *tmp = (seq_t *)realloc(cache, (new_cap + 1) * sizeof(seq_t));
        if (!tmp) {
            free(cache);
            exit(EXIT_FAILURE);
        }
        cache = tmp;

        for (idx_t i = cap + 1; i <= new_cap; ++i) {
            if (i <= 36) {
                /* Quadratic recurrence for the first 36 terms */
                seq_t num = cache[i - 1] * cache[i - 12]
                            + cache[i - 6] + cache[i - 7];
                seq_t den = cache[i - 13];
                cache[i] = num / den; /* exact division by construction */
            } else {
                /* Linear recurrence: a(n) = 124*(a(n-12)-a(n-24)) + a(n-36) */
                seq_t diff = cache[i - 12] - cache[i - 24];
                seq_t result = cache[i - 36];
                /* Use repeated addition to avoid multiplication overflow */
                for (int k = 0; k < 124; ++k)
                    result += diff;
                cache[i] = result;
            }
        }
        cap = new_cap;
    }

    return cache[n];
}
