/*
 * A168322  a(1)=3,a(2)=5; a(n+1)=smallest prime number > a(n-1) such that the
 *          sum of any three consecutive terms is a prime.
 * https://oeis.org/A168322
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1)=3, a(2)=5; for n≥3, * a(n)=the smallest prime greater than
 *            a(n‑2) * that makes a(n‑2)+a(n‑1)+a(n) also prime
 * Range:     u8 n <= 23, u16 n <= 59, u32 n <= 59, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Helpers:   ensure_cache, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/is_prime_big.h"
#include "../utils/ensure_cache.h"

seq_t a(idx_t n)
{
    /* static cache of computed terms, 1‑based indices stored in 0‑based array */
    static seq_t *terms = NULL;
    static size_t terms_cap = 0;   /* current allocated capacity */
    static idx_t terms_len = 0;    /* number of terms already computed (≤ terms_cap) */

    if (n == 0) return 0; /* undefined, but guard against illegal argument */

    /* grow cache if necessary */
    if (terms_len < n)
    {
        /* ensure enough space for n terms */
        ensure_cache(n, &terms, &terms_cap);

        /* compute missing terms */
        for (idx_t i = terms_len + 1; i <= n; ++i)
        {
            if (i == 1)
                terms[i - 1] = (seq_t)3;        /* a(1) */
            else if (i == 2)
                terms[i - 1] = (seq_t)5;        /* a(2) */
            else
            {
                seq_t prev2 = terms[i - 3];     /* a(i‑2) */
                seq_t prev1 = terms[i - 2];     /* a(i‑1) */
                seq_t candidate = prev2 + (seq_t)1; /* start just above prev2 */

                for (;; ++candidate)
                {
                    if (!is_prime_big(candidate))
                        continue;

                    seq_t sum = prev2 + prev1 + candidate;
                    if (is_prime_big(sum))
                    {
                        terms[i - 1] = candidate; /* a(i) */
                        break;
                    }
                }
            }
        }

        terms_len = n; /* update length after filling up to n */
    }

    return terms[n - 1];
}
