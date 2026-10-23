/*
 * A170258  Number of reduced words of length n in Coxeter group on 9
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^41 = I.
 * https://oeis.org/A170258
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1, a(n) = 9*8^(n-1) for n >= 1
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 19, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 1;

    seq_t result = 1;
    seq_t base = 8;
    idx_t exp = n - 1;          /* exponent (>=1) */

    while (exp)
    {
        if (exp & 1) result *= base;
        base *= base;
        exp >>= 1;
    }

    result *= 9;
    return result;
}
