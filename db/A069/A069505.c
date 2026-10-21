/*
 * A069505  a(1) = 1; a(n) = smallest palindromic number of the form k*a(n-1)
 *          + 1 with k > 1.
 * https://oeis.org/A069505
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(0)=1, a(1)=3, a(2)=7, a(3)=22; a(n) = (10^(n-1) - 1) / 9 for n
 *            >= 4 (i.e. the repunit with n-1 digits)
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 11, u64 n <= 21, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Small indices are defined by construction. */
    if (n == 0) return 1;
    if (n == 1) return 3;
    if (n == 2) return 7;
    if (n == 3) return 22;

    /* For n >= 4, a(n) is the repunit of length n-1. */
    idx_t len = n - 1;          /* number of digits '1' */
    seq_t r = 0;
    for (idx_t i = 0; i < len; ++i)
        r = r * (seq_t)10 + 1;
    return r;
}
