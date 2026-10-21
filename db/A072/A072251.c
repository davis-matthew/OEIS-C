/*
 * A072251  (3*a(n)+1)/2^(2*n + 1) = 23-6*n.
 * https://oeis.org/A072251
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy, less
 * Formula:   a(n) = ((46-12*n)*4^n-1)/3
 * Range:     i8 n <= 2, i16 n <= 5, i32 n <= 12, i64 n <= 22, i128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes 4^n in seq_t; works for any idx_t up to reasonably large n. */
static seq_t pow4(idx_t n)
{
    seq_t p = 1;
    for (idx_t i = 0; i < n; ++i)
        p *= 4;
    return p;
}

seq_t a(idx_t n)
{
    /* ((46-12*n)*4^n-1)/3  */
    seq_t term = (seq_t)46 - ((seq_t)12 * (seq_t)n);
    seq_t p = pow4(n);
    seq_t res = (term * p - 1) / 3;
    return res;
}
