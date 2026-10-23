/*
 * A176916  a(n) = 5^n + 5*n + 1.
 * https://oeis.org/A176916
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 5^n + 5*n + 1
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 13, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t pow5 = 1;
    seq_t base = 5;
    idx_t exp = n;

    while (exp > 0) {
        if (exp & 1) pow5 *= base;
        base *= base;
        exp >>= 1;
    }

    seq_t result = pow5 + ((seq_t)5 * n) + 1;
    return result;
}
