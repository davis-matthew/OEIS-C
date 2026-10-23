/*
 * A138119  Concatenation of n digits 1 and 2*n-1 digits 0.
 * https://oeis.org/A138119
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = 10^(2*n-1) * (10^n - 1) / 9
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 3, u64 n <= 7, u128 n <= 12,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* compute 10^n */
    seq_t pow10n = 1;
    seq_t base = 10;
    idx_t exp = n;
    while (exp) {
        if (exp & 1) pow10n *= base;
        base *= base;
        exp >>= 1;
    }
    /* compute 10^(2*n-1) */
    seq_t pow10_2n_minus1 = 1;
    base = 10;
    exp = 2 * n - 1;
    while (exp) {
        if (exp & 1) pow10_2n_minus1 *= base;
        base *= base;
        exp >>= 1;
    }
    /* (10^n - 1) / 9 gives the integer with n ones */
    seq_t ones = (pow10n - 1) / 9;
    /* concatenate the zeros by multiplication with 10^(2*n-1) */
    return ones * pow10_2n_minus1;
}
