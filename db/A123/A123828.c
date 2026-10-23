/*
 * A123828  Orientable embedding number gamma_1 for bouquet B_n.
 * https://oeis.org/A123828
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, more
 * Formula:   a(2) = 2, a(n) = a(n-1) * ((4*n-4)*(2*n-1))/(n-2) for n >= 3
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 5, u64 n <= 5, u128 n <= 5,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n < 2) return (seq_t)0;   /* values below the offset are irrelevant */
    if (n == 2) return (seq_t)2;
    seq_t res = (seq_t)2;
    for (idx_t k = 3; k <= n; ++k) {
        seq_t num = ((seq_t)4 * (seq_t)k - 4) * (((seq_t)2 * (seq_t)k) - 1);
        seq_t den = k - 2;
        seq_t factor = num / den;   /* integral division: the numerator is always divisible by den */
        res *= factor;
    }
    return res;
}
