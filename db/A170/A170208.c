/*
 * A170208  Number of reduced words of length n in Coxeter group on 7
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^40 = I.
 * https://oeis.org/A170208
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1, n = 0 a(n) = 7 * 6^(n-1), n >= 1
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) {
        return (seq_t)1;
    }

    /* Compute 6^(n-1) */
    seq_t power = (seq_t)6;
    seq_t result = (seq_t)1;
    idx_t exp = n - 1;
    while (exp) {
        if (exp & 1)
            result *= power;
        if (exp > 1)   /* avoid extra multiplication when exp becomes 1 */
            power *= power;
        exp >>= 1;
    }

    /* Multiply by 7 for n >= 1 */
    result *= (seq_t)7;
    return result;
}
