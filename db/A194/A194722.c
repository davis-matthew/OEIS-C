/*
 * A194722  Number of n-ary words beginning with the first character of the
 *          alphabet, that can be built by inserting ten doublets into the
 *          initially empty word.
 * https://oeis.org/A194722
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 0, for n>0 * a(n) = 1 + (18 + (152 + (798 + (2907 + (7752
 *            + (15504 + (23256 + (25194 +
 *            16796*(n-1))*(n-1))*(n-1))*(n-1))*(n-1))*(n-1))*(n-1))*(n-1))*(n-1)
 *            * Computed by nested Horner evaluation
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 4, u64 n <= 47, u128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;

    seq_t x = (seq_t)n - 1;           /* x = n-1 */
    seq_t t = (seq_t)16796 * x;       /* inner most term 16796*(n-1) */

    t = (t + (seq_t)25194) * x;
    t = (t + (seq_t)23256) * x;
    t = (t + (seq_t)15504) * x;
    t = (t + (seq_t)7752) * x;
    t = (t + (seq_t)2907) * x;
    t = (t + (seq_t)798) * x;
    t = (t + (seq_t)152) * x;
    t = (t + (seq_t)18) * x;

    return (seq_t)1 + t;
}
