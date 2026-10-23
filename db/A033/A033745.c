/*
 * A033745  Expansion of Product_{d | 33} theta_3(q^d).
 * https://oeis.org/A033745
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = coefficient of q^n in ∏_{d | 33} θ₃(q^d), where θ₃(q) =
 *            Σ_{m∈ℤ} q^{m² } (OEIS A033745)
 * Range:     u8 n <= 287, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* Allocate arrays to hold intermediate convolution results */
    seq_t *cur  = (seq_t *)calloc(n + 1, sizeof(seq_t));
    seq_t *next = (seq_t *)calloc(n + 1, sizeof(seq_t));
    if (!cur || !next) {
        /* In an error situation return 0 (sequence values are non‑negative) */
        free(cur);
        free(next);
        return 0;
    }

    /* Start with 1 (empty sum contributes only q⁰) */
    cur[0] = 1;

    /* Divisors of 33 */
    const idx_t divisors[4] = {1U, 3U, 11U, 33U};

    /* Convolve with each theta series */
    for (int d_idx = 0; d_idx < 4; ++d_idx) {
        const idx_t d = divisors[d_idx];

        /* Zero the destination buffer */
        for (idx_t i = 0; i <= n; ++i) next[i] = 0;

        /* For every current coefficient, add contributions from θ₃(q^d) */
        for (idx_t i = 0; i <= n; ++i) {
            seq_t val = cur[i];
            if (!val) continue;          /* skip zero contributions */

            /* Add terms m=0,1,2,… while the resulting exponent stays ≤ n */
            for (idx_t m = 0; ; ++m) {
                idx_t add = d * m * m;   /* d·m²  */
                /* Ensure i + add ≤ n without overflow */
                if (add > n - i) break;
                /* coefficient of q^{d·m²} in θ₃(q^d) :
                   1 for m=0, 2 for m≠0  */
                seq_t coeff = (m == 0) ? 1 : 2;
                next[i + add] += val * coeff;
            }
        }

        /* Prepare for next divisor: cur ← next, next ← old cur */
        seq_t *tmp  = cur;
        cur = next;
        next = tmp;
    }

    /* Result is in cur[n] */
    seq_t result = cur[n];

    /* Free the allocated buffers */
    free(cur);
    free(next);

    return result;
}
