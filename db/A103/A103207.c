/*
 * A103207  a(n)=(-1)^floor(n/2)/det(M_n) where M_n is the n X n matrix of
 *          terms 1/(i+j)! i and j ranging from 1 to n.
 * https://oeis.org/A103207
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (1/2^n)*{prod(k=1, n, (2*k)!/k!)}^2
 * Range:     u8 n <= 2, u16 n <= 2, u32 n <= 3, u64 n <= 4, u128 n <= 6,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t a_val = 1;
    if (n == 0) return a_val;
    seq_t F = 1;
    for (idx_t i = 1; i <= n; ++i) {
        /* Update F = (F * (2*i-1)*(2*i)) / i */
        seq_t mult = (seq_t)(2 * i - 1) * (seq_t)(2 * i);
        F = (F * mult) / (seq_t)i;
        /* Update a = a_val * (F*F) / 2 */
        seq_t prod = F * F;
        a_val = (a_val * prod) / 2;
    }
    return a_val;
}
