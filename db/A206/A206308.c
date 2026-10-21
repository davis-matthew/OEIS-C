/*
 * A206308  a(n) = ((2*n+2)*(2*n+3) - 1)*a(n-1) + 2*n*(2*n+1)*a(n-2), a(0)=1,
 *          a(1)=19.
 * https://oeis.org/A206308
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = ((2*n+2)*(2*n+3)-1)*a(n-1) + 2*n*(2*n+1)*a(n-2) a(0) = 1,
 *            a(1) = 19
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 5, u64 n <= 9, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)19;

    seq_t a0 = (seq_t)1;
    seq_t a1 = (seq_t)19;

    for (idx_t i = 2; i <= n; ++i) {
        seq_t ii = (seq_t)i;
        seq_t two_i = ii * (seq_t)2;
        seq_t coeff1 = (two_i + (seq_t)2) * (two_i + (seq_t)3) - (seq_t)1;  // (2i+2)*(2i+3)-1
        seq_t coeff2 = two_i * (two_i + (seq_t)1);                           // 2i*(2i+1)
        seq_t new_val = coeff1 * a1 + coeff2 * a0;
        a0 = a1;
        a1 = new_val;
    }

    return a1;
}
