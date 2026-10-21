/*
 * A070252  Number of n-digit palindromes.
 * https://oeis.org/A070252
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn, easy
 * Formula:   a(1) = 10 For n > 1: if n is odd: a(n) = 9 * 10^((n-1)/2) if n
 *            is even: a(n) = 9 * 10^(n/2-1)
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 18, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 1) {
        return (seq_t)10;
    }

    /* Determine exponent of 10 */
    idx_t exp;
    if (n & 1U) {                 /* n is odd */
        exp = (n - 1U) / 2U;
    } else {                       /* n is even */
        exp = (n / 2U) - 1U;
    }

    /* Compute 10^exp */
    seq_t pow = (seq_t)1;
    for (idx_t i = 0; i < exp; ++i) {
        pow *= (seq_t)10;
    }

    /* Multiply by 9 to get the count */
    return pow * (seq_t)9;
}
