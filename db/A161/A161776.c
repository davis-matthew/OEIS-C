/*
 * A161776  Number of reduced words of length n in the Weyl group B_11.
 * https://oeis.org/A161776
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, fini, full
 * Formula:   a(n) = coefficient of x^n in the product ∏_{k=1}^{11} (1 + x + …
 *            + x^(2k‑1)) (max degree 121)
 * Range:     u8 n <= 2, u16 n <= 8, u32 n <= 121, u64 n <= 121, u128 n <= 121,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* maximum degree for B_11 is 11² = 121 */
    const idx_t MAX_DEG = (idx_t)121;
    static seq_t coeff[122];
    static seq_t temp[122];
    static int initialized = 0;

    if (!initialized) {
        /* coeff[0] = 1, all others are zero by static initialization */
        coeff[0] = (seq_t)1;
        idx_t curr_deg = 0;          /* highest degree currently in coeff */

        for (idx_t k = (idx_t)1; k <= (idx_t)11; ++k) {
            idx_t factor_len = 2 * k;                  /* 1 + x + ... + x^(2k‑1) */
            idx_t new_deg = curr_deg + factor_len - 1;

            /* zero temp up to the new maximum degree */
            for (idx_t i = (idx_t)0; i <= new_deg; ++i)
                temp[i] = (seq_t)0;

            /* multiply current coeff by the factor */
            for (idx_t i = (idx_t)0; i <= curr_deg; ++i) {
                if (coeff[i] == (seq_t)0) continue;
                for (idx_t j = (idx_t)0; j < factor_len; ++j)
                    temp[i + j] += coeff[i];
            }

            /* copy temp back to coeff for next iteration */
            for (idx_t i = (idx_t)0; i <= new_deg; ++i)
                coeff[i] = temp[i];

            curr_deg = new_deg;
        }
        initialized = 1;
    }

    if (n > MAX_DEG)
        return (seq_t)0;
    return coeff[n];
}
