/*
 * A170478  Number of reduced words of length n in Coxeter group on 37
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^45 = I.
 * https://oeis.org/A170478
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 35*Σ_{i=1}^{44} a(n-i) - 630*a(n-45) for n≥45, with
 *            a(0)=1, a(1)=37, a(2)=1332, a(3)=47952, … determined by the
 *            rational generating function given in the OEIS entry. The first
 *            45 values are computed from the equation c_n =
 *            Σ_{k=0}^{min(n,45)} d_k a(n-k) where d_0=1, d_1..d_44=−35,
 *            d_45=630...
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 12, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* static cache for generated terms */
    static seq_t *cache = NULL;
    static size_t cache_size = 0;
    static idx_t computed = 0;   /* highest index already stored */

    /* enlarge cache if necessary */
    if ((idx_t)(cache_size) <= n) {
        size_t new_size = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp) { abort(); }          /* allocation failed */
        cache     = tmp;
        cache_size = new_size;
    }

    /* initialise a(0) if not yet done */
    if (computed == 0 && cache_size > 0) {
        cache[0] = (seq_t)1;
    }

    /* generate terms up to n */
    if (n > computed) {
        for (idx_t i = computed + 1; i <= n; ++i) {
            seq_t val;

            /* ---------- first 45 terms (i ≤ 45) ---------- */
            if (i <= 45) {
                seq_t sum = (seq_t)0;
                idx_t kmax = i;                 /* k runs 1 … i */
                for (idx_t k = 1; k <= kmax; ++k) {
                    seq_t t = cache[i - k];
                    /* d_k :  -35 for k=1..44,  +630 for k=45 */
                    if (k <= 44) {
                        /* -35 * t  →  -(35*t) */
                        sum += -((seq_t)35 * t);
                    } else { /* k == 45 */
                        sum += (seq_t)630 * t;
                    }
                }
                seq_t c = (seq_t)2;              /* numerator coefficient c_i */
                if (i == 0 || i == 45) c = (seq_t)1;
                val = c - sum;                  /* a_i = c_i - Σ d_k a_{i-k} */
            }
            /* ---------- remaining terms (i ≥ 46) ---------- */
            else {
                seq_t sum35 = (seq_t)0;
                for (idx_t k = 1; k <= 44; ++k) {
                    sum35 += (seq_t)35 * cache[i - k];
                }
                seq_t sum630 = (seq_t)630 * cache[i - 45];
                val = sum35 - sum630;           /* a_i = 35*Σ a_{i-k} - 630*a_{i-45} */
            }

            cache[i] = val;
        }
        computed = n;
    }

    return cache[n];
}
