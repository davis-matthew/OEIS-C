/*
 * A168868  Number of reduced words of length n in Coxeter group on 47
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^20 = I.
 * https://oeis.org/A168868
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1 for 1 ≤ n ≤ 19: a(n) = 45·∑_{i=0}^{n-1} a(i) + 2 a(20)
 *            = 45·∑_{i=0}^{19} a(i) – 1035·a(0) + 1 for n ≥ 21: a(n) =
 *            45·∑_{i=n-19}^{n-1} a(i) – 1035·a(n−20) (derived from the
 *            rational generating function of the group)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 11, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* static cache of computed terms and their prefix sums  */
    static seq_t *a_cache = NULL;
    static seq_t *s_cache = NULL;   /* prefix sums: s_cache[i] = Σ_{j=0}^{i} a_cache[j] */
    static idx_t cache_len = 0;     /* number of elements currently cached (>=1 if a_cache != NULL) */

    /* allocate space if needed   */
    if (n + 1 > cache_len) {
        idx_t new_len = n + 1U;
        seq_t *new_a = (seq_t *)realloc(a_cache, (size_t)new_len * sizeof(seq_t));
        seq_t *new_s = (seq_t *)realloc(s_cache, (size_t)new_len * sizeof(seq_t));
        if (!new_a || !new_s) return (seq_t)0;   /* allocation failed – return 0 */

        a_cache = new_a;
        s_cache = new_s;

        /* initialise cache if this is the first allocation */
        if (cache_len == 0) {
            a_cache[0] = (seq_t)1;
            s_cache[0] = (seq_t)1;
            cache_len = 1U;
        }

        /* compute all new terms up to n */
        for (idx_t k = cache_len; k <= n; ++k) {
            if (k <= 19U) {                           /* 1 … 19  */
                a_cache[k] =
                    (seq_t)45 * s_cache[k - 1U] + (seq_t)2;
            } else if (k == 20U) {                    /* n = 20   */
                a_cache[20U] =
                    (seq_t)45 * s_cache[19U] - (seq_t)1035 * a_cache[0] + (seq_t)1;
            } else {                                  /* n ≥ 21   */
                a_cache[k] =
                    (seq_t)45 * (s_cache[k - 1U] - s_cache[k - 20U])
                    - (seq_t)1035 * a_cache[k - 20U];
            }
            s_cache[k] = s_cache[k - 1U] + a_cache[k];
        }
        cache_len = new_len;
    }
    return a_cache[n];
}
