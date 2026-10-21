/*
 * A228921  Number of solutions to Sum_{i=1..n} x_i^2 == 0 (mod 8) with x_i in
 *          0..7.
 * https://oeis.org/A228921
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = number of n‑tuples (x₁,…,xₙ) with each xᵢ∈{0,…,7}
 *            satisfying ∑ xᵢ² ≡ 0 (mod 8)
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 11, u64 n <= 22, u128 n <= 43,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    /* Each component contributes either residue 0 (2 choices),
       residue 1 (4 choices), or residue 4 (2 choices). */
    seq_t f[8] = {0};          /* current counts for residues 0..7 */
    seq_t new_f[8];
    f[0] = 1;                  /* start with empty sum = 0 */

    for (idx_t i = 0; i < n; ++i) {
        /* zero new_f */
        for (int k = 0; k < 8; ++k) new_f[k] = 0;

        /* add residue 0 (mult. by 2) */
        for (int k = 0; k < 8; ++k) {
            new_f[k] += f[k] * 2U;
        }
        /* add residue 1 (mult. by 4) */
        for (int k = 0; k < 8; ++k) {
            new_f[(k + 1) & 7] += f[k] * 4U;
        }
        /* add residue 4 (mult. by 2) */
        for (int k = 0; k < 8; ++k) {
            new_f[(k + 4) & 7] += f[k] * 2U;
        }

        /* copy new_f back to f for next iteration */
        for (int k = 0; k < 8; ++k) f[k] = new_f[k];
    }

    return f[0];   /* count of tuples with sum ≡ 0 mod 8 */
}
