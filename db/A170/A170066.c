/*
 * A170066  Number of reduced words of length n in Coxeter group on 9
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^37 = I.
 * https://oeis.org/A170066
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 if n=0, otherwise 9 * 8^(n-1)
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 19, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;

    /* exponentiation by squaring: compute 8^(n-1) */
    seq_t result = (seq_t)1;
    seq_t base   = (seq_t)8;
    idx_t e = n - 1;

    while (e > 0) {
        if ((e & 1) != 0)
            result *= base;
        e >>= 1;
        if (e > 0)          /* avoid an extra square when e becomes 0 */
            base *= base;
    }

    return result * (seq_t)9;
}
