/*
 * A033732  Expansion of Product_{d | 20} theta_3(q^d).
 * https://oeis.org/A033732
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the coefficient of q^n in ∏_{d|20} θ3(q^d) where θ3(q) =
 *            Σ_{k∈ℤ} q^{k^2}
 * Range:     u8 n <= 36, u16 n <= 547, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 66 terms through n = 9739 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static void seq_free_arr(seq_t *ptr) {
#ifdef __cplusplus
    delete[] ptr;
#else
    free(ptr);
#endif
}

static seq_t* seq_malloc_zero(idx_t len) {
#ifdef __cplusplus
    return new seq_t[len]();          /* value‑initialised (zero) */
#else
    return (seq_t*)calloc((size_t)len, sizeof(seq_t));
#endif
}

seq_t a(idx_t n) {
    /* Divisors of 20: 1,2,4,5,10,20 */
    static const idx_t divisors[] = {1, 2, 4, 5, 10, 20};
    const size_t D = sizeof(divisors) / sizeof(divisors[0]);

    /* initial array representing θ3(q^1) */
    seq_t *cur = seq_malloc_zero(n + 1);
    cur[0] = (seq_t)1;                 /* only q^0 term */

    for (size_t di = 0; di < D; ++di) {
        idx_t d = divisors[di];
        seq_t *next = seq_malloc_zero(n + 1);

        for (idx_t k = 0; k <= n; ++k) {
            /* t = 0  ── coefficient 1  */
            next[k] += cur[k];

            /* t > 0  ── coefficient 2  */
            idx_t t = 1;
            while (1) {
                unsigned __int128 prod = (unsigned __int128)t * t * d;
                if (prod > k) break;
                seq_t val = cur[k - (idx_t)prod];
                next[k] += val;   /* first copy  */
                next[k] += val;   /* second copy  */
                ++t;
            }
        }

        seq_free_arr(cur);
        cur = next;
    }

    seq_t result = cur[n];
    seq_free_arr(cur);
    return result;
}
