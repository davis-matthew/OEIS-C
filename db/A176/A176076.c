/*
 * A176076  Sequence defined by the recursion a(n)=(1+a(n-1-(n mod
 *          a(n-1)))-(-1)^n*a(n-1)) mod n, with a(1)=0.
 * https://oeis.org/A176076
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 0 a(n) = (1 + a(n-1-(n mod (a(n-1)+1))) - (-1)^n *
 *            a(n-1)) mod n (n≥2)
 * Range:     u8 n <= 95, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static cache of already computed values.
       cache[i] holds a(i+1) for i = 0..cache_len-1. */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;

    if (n == 0)          /* defensive: n should never be 0 */
        return 0;

    /* Grow cache to at least n elements if necessary. */
    if (cache_len < n) {
        seq_t *tmp = (seq_t *)realloc(cache, n * sizeof(seq_t));
        if (!tmp) {      /* allocation failed – return 0 */
            free(cache);
            cache = NULL;
            cache_len = 0;
            return 0;
        }
        cache = tmp;

        /* Compute missing terms from cache_len up to n-1 (0‑based). */
        for (idx_t i = cache_len; i < n; ++i) {
            idx_t curr_n = i + 1;   /* current index, 1‑based */

            if (curr_n == 1) {      /* a(1) = 0 */
                cache[i] = 0;
                continue;
            }

            seq_t prev = cache[i - 1];                     /* a(n-1) */
            seq_t divisor = prev + 1;                      /* a(n-1)+1  */
            seq_t mod = ((seq_t)curr_n) % divisor;         /* n mod divisor */

            /* k = n-1 - (n mod divisor)  (k ≥ 1) */
            idx_t k = (curr_n - 1) - (idx_t)mod;
            seq_t ak = cache[k - 1];                        /* a(k) */

            /* Compute (1 + ak + (± prev)) modulo curr_n.
               Use modular arithmetic to avoid overflow. */
            seq_t val = 1;
            val = (val + ak) % (seq_t)curr_n;
            if (curr_n & 1ULL) {          /* n odd: + prev */
                val = (val + prev) % (seq_t)curr_n;
            } else {                      /* n even: - prev  ≡ + (n - prev) */
                seq_t tmp_sub = (seq_t)curr_n - prev;
                val = (val + tmp_sub) % (seq_t)curr_n;
            }

            cache[i] = val;
        }
        cache_len = n;
    }

    return cache[n - 1];
}
