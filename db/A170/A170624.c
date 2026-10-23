/*
 * A170624  Number of reduced words of length n in Coxeter group on 39
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^48 = I.
 * https://oeis.org/A170624
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1, a(n) = 39 * 38^(n-1) for n ≥ 1 (Number of reduced
 *            words of length n in a Coxeter group with 39 generators and
 *            relations S_i^2 = (S_i S_j)^48 = I.)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 12, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return 1;

    seq_t result = 39;          /* first generator choice */
    seq_t base = 38;            /* choices for each subsequent position */
    idx_t exp = n - 1;          /* number of remaining positions */

    /* binary exponentiation */
    while (exp > 0) {
        if (exp & 1)
            result *= base;
        exp >>= 1;
        if (exp)
            base *= base;
    }
    return result;
}
