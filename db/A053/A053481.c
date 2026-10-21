/*
 * A053481  First differences of A029767.
 * https://oeis.org/A053481
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n+1)! * (2^(n+2) - 1) - n! * (2^(n+1) - 1)
 *            (equivalently, a(n) = n! * ((2n+1)*2^(n+1) - n) )
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 15, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Compute n! and 2^n */
    seq_t fact = (seq_t)1;
    seq_t pow2 = (seq_t)1;
    for (idx_t i = (idx_t)1; i <= n; ++i) {
        fact *= (seq_t)i;   /* factorial */
        pow2 <<= 1;         /* power of two */
    }

    /* Derivative factor: (2n+1)*2^(n+1) - n   (2^(n+1) = pow2 << 1) */
    seq_t dbl_pow2 = pow2 << 1;                       /* 2^(n+1) */
    seq_t term_factor = ((seq_t)(2 * n + 1)) * dbl_pow2 - (seq_t)n;

    /* Result */
    return fact * term_factor;
}
