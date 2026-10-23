/*
 * A161528  Expansion of the q-series Sum_{n >= 0}
 *          (-1)^nq^(n(n+1)/2)(1-q)(1-q^2)...(1-q^n)/((1-q^(n+1))(1-q^(n+2))...(1-q^(2n+1))).
 * https://oeis.org/A161528
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 99, u16 n <= 99, u32 n <= 99, u64 n <= 99, u128 n <= 99,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) = coefficient of q^n in 
 *   Σ_{m≥0} (-1)^m q^{m(m+1)/2} (1-q)(1-q^2)…(1-q^m)
/* 
 *                / (1-q^{m+1})(1-q^{m+2})…(1-q^{2m+1}) 
 *
 * The sequence is also equal to A035187(5n+1).  The implementation below
 * expands this q‑series directly, computing each needed coefficient by
 * polynomial multiplication.  All arithmetic is performed in signed
 * 64‑bit integers; the final result (which is non‑negative and very
 * small) is cast to seq_t.
 */

#include <stdint.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* maximum degree required */
    size_t N = (size_t)n;

    /* table for accumulating the final coefficients */
    int64_t *coeff = (int64_t *)calloc(N + 1, sizeof(int64_t));
    if (!coeff) exit(EXIT_FAILURE);

    /* temporary polynomial for the m‑th summand */
    int64_t *tmp = NULL;
    size_t tmp_capacity = 0;

    for (idx_t m = 0;; ++m) {
        uint64_t shift = ((uint64_t)m * (m + 1)) / 2;
        if (shift > N) break;

        size_t max_deg = N - (size_t)shift;          /* polynomial length – 1 */
        if (max_deg + 1 > tmp_capacity) {
            free(tmp);
            tmp = (int64_t *)calloc(max_deg + 1, sizeof(int64_t));
            if (!tmp) { free(coeff); exit(EXIT_FAILURE); }
            tmp_capacity = max_deg + 1;
        } else {
            /* zero the part that will be used */
            for (size_t i = 0; i <= max_deg; ++i) tmp[i] = 0;
        }

        tmp[0] = 1;                                   /* start polynomial */

        /* multiply by (1 - q^k) for k = 1 .. m */
        for (int k = 1; k <= (int)m; ++k) {
            for (int i = (int)max_deg; i >= k; --i) {
                tmp[i] -= tmp[i - k];
            }
        }

        /* multiply by 1/(1 - q^k) for k = m+1 .. 2m+1 */
        for (int k = (int)m + 1; k <= (int)(2 * m + 1); ++k) {
            for (int i = k; i <= (int)max_deg; ++i) {
                tmp[i] += tmp[i - k];
            }
        }

        /* add this summand to the final coefficient array */
        int sign = ((m & 1u) == 0) ? 1 : -1;
        for (size_t i = 0; i <= max_deg; ++i) {
            int64_t val = tmp[i];
            if (sign == 1)
                coeff[shift + i] += val;
            else
                coeff[shift + i] -= val;
        }
    }

    seq_t result = (seq_t)coeff[N];

    free(tmp);
    free(coeff);
    return result;
}
