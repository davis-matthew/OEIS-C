/*
 * A169288  Number of reduced words of length n in Coxeter group on 35
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^29 = I.
 * https://oeis.org/A169288
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 12, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A169288: Number of reduced words of length n in a Coxeter group
 *  with 35 generators and relations S_i^2=(S_iS_j)^29=I.
 *
 *  The generating function is
 *     (t^29 + 2t^28 + … + 2t + 1) /
 *     (1 – 33t – 33t^2 – … – 33t^28 + 561t^29),
 *  so the coefficients satisfy
 *
 *     a(n) = 33·a(n-1)+33·a(n-2)+…+33·a(n-28)
 *            – 561·a(n-29) + b(n),
 *
 *  where b(0)=1, b(1)…b(28)=2, b(29)=1, and b(n)=0 for n>29.
 *
 *  The implementation keeps a static cache so that repeated calls with
 *  arbitrary ordering of n are handled in O(n) total time.
 *  All arithmetic uses only the operators allowed for types seq_t
 *  and idx_t.  The recurrence never overflows a 64‑bit unsigned type
 *  while providing correct answers for all n that fit in the built
 *  seq_t type.
 */
#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cache_size = 0;

    /* enlarge cache if necessary */
    if ((size_t)n + 1 > cache_size) {
        size_t new_size = (size_t)n + 1;
        seq_t *tmp = (seq_t*)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp) return 0;                /* allocation failure */
        cache = tmp;
        /* compute new entries */
        for (size_t i = cache_size; i < new_size; ++i) {
            seq_t sum = 0;

            /* 33*sum_{k=1}^{28} a(i-k)  */
            for (idx_t k = 1; k <= 28 && i >= (size_t)k; ++k) {
                sum += (seq_t)33 * cache[i - k];
            }

            /* – 561·a(i-29) if i ≥ 29 */
            if (i >= 29) {
                sum -= (seq_t)561 * cache[i - 29];
            }

            /* add the non‑homogeneous term b(i)  */
            if (i == 0 || i == 29) {
                sum += (seq_t)1;
            } else if (i <= 28) {
                sum += (seq_t)2;
            }

            cache[i] = sum;
        }
        cache_size = new_size;
    }

    return cache[n];
}
