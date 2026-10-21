/*
 * A087463  Generalized multiplicative Jacobsthal sequence.
 * https://oeis.org/A087463
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (2^n - (-1)^n)/3 for n mod 3 ≠ 0, otherwise a(n) = 0
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 33, u64 n <= 66, u128 n <= 129,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Terms that are multiples of 3 are zero. */
    if ((n % 3) == 0)
        return 0;

    /* Compute 2^n.  Left‑shifting by 1 multiplies by 2. */
    seq_t two_pow = 1;
    for (idx_t i = 0; i < n; ++i)
        two_pow <<= 1;

    /* For odd n we need (2^n + 1)/3, for even n (2^n - 1)/3. */
    if (n & 1)          /* n is odd */
        return (two_pow + 1) / 3;
    else                /* n is even */
        return (two_pow - 1) / 3;
}
