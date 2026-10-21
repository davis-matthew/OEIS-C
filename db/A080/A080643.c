/*
 * A080643  a(0)=0; for n>0, a(n) = 4^n - 2*3^(n-1).
 * https://oeis.org/A080643
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=0; a(n)=4^n - 2*3^(n-1) for n>0
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 32, u128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* a(0) is defined to be 0 */
    if (n == 0) {
        return (seq_t)0;
    }

    /* Compute 4^n */
    seq_t pow4 = (seq_t)1;
    for (idx_t i = 0; i < n; ++i) {
        pow4 *= (seq_t)4;
    }

    /* Compute 3^(n-1) (for n>0 the exponent is at least 0) */
    seq_t pow3 = (seq_t)1;
    for (idx_t i = 0; i < n - 1; ++i) {
        pow3 *= (seq_t)3;
    }

    /* Compute 2*3^(n-1) */
    seq_t two_pow3 = pow3 * (seq_t)2;

    /* a(n) = 4^n - 2*3^(n-1) */
    return pow4 - two_pow3;
}
