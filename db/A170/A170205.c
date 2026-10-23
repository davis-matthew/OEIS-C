/*
 * A170205  Number of reduced words of length n in Coxeter group on 4
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^40 = I.
 * https://oeis.org/A170205
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=1; a(n)=4*3^(n-1) for n>=1
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 19, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;

    /* fast exponentiation: compute 3^(n-1) */
    seq_t result = (seq_t)1;
    seq_t base   = (seq_t)3;
    idx_t exp    = n - 1;
    while (exp > 0) {
        if (exp & 1)
            result = result * base;
        base   = base * base;
        exp >>= 1;
    }

    /* multiply by 4 */
    result = result * (seq_t)4;
    return result;
}
