/*
 * A170329  Number of reduced words of length n in Coxeter group on 32
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^42 = I.
 * https://oeis.org/A170329
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 (n=0), else a(n) = 32 * 31^(n-1)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 12, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;     /* a(0) = 1 */

    seq_t r = (seq_t)32;             /* start with 32 for n>=1 */
    for (idx_t i = 1; i < n; ++i)    /* multiply by 31 for n-1 times */
        r *= (seq_t)31;

    return r;
}
