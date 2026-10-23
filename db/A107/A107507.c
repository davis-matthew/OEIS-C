/*
 * A107507  Coordination sequence for D_12 lattice.
 * https://oeis.org/A107507
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Σ_{i=0}^{12} c_i · C(n - i + 11, 11) (C = binomial
 *            coefficient)
 * Range:     u8 n <= 0, u16 n <= 2, u32 n <= 8, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static const seq_t coeff[13] = {
    (seq_t)1,      (seq_t)252,   (seq_t)10386,  (seq_t)133516,
    (seq_t)732591, (seq_t)1956216,(seq_t)2698108, (seq_t)1956216,
    (seq_t)732591, (seq_t)133516, (seq_t)10386,  (seq_t)252,
    (seq_t)1
};

seq_t a(idx_t n)
{
    seq_t res = 0;
    for (idx_t i = 0; i <= 12; ++i) {
        if (i > n) break;                 /* terms with i > n contribute 0 */
        idx_t m = n - i + 11;              /* m >= 11 always */

        /* compute binomial C(m,11) using 128‑bit intermediate */
        unsigned __int128 bin = 1;
        for (idx_t k = 1; k <= 11; ++k) {
            bin = bin * (m - 11 + k);     /* multiply by (m-10)...m */
            bin /= k;                     /* divide by k, exact */
        }

        unsigned __int128 prod = (unsigned __int128)coeff[i] * bin;
        res += (seq_t)prod;
    }
    return res;
}
