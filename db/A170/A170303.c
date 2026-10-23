/*
 * A170303  Number of reduced words of length n in Coxeter group on 6
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^42 = I.
 * https://oeis.org/A170303
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 if n==0, else 6 * 5^(n-1)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 13, u64 n <= 21, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;

    seq_t result = (seq_t)6;      /* start with 6 = 6*5^0 */
    seq_t base   = (seq_t)5;      /* base = 5  */
    idx_t e = n - 1;              /* exponent 5^(n-1) */

    while (e) {
        if (e & 1U) result *= base;
        base *= base;
        e >>= 1;
    }

    return result;
}
