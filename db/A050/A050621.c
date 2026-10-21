/*
 * A050621  Smallest n-digit number divisible by 2^n.
 * https://oeis.org/A050621
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = 10^(n-1) + 2^(n-1) (for n >= 1)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 10, u64 n <= 20, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Safety for n == 0: not part of the official sequence, return 0 */
    if (n == 0) return 0;

    seq_t pow10 = 1;
    seq_t pow2  = 1;

    /* compute 10^(n-1) */
    for (idx_t i = 1; i < n; ++i)
        pow10 *= 10;

    /* compute 2^(n-1) */
    for (idx_t i = 1; i < n; ++i)
        pow2 *= 2;

    return pow10 + pow2;
}
