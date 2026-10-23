/*
 * A138750  a(n) = ceiling(n/2) if n == 2 (mod 3), a(n) = 2n otherwise.
 * https://oeis.org/A138750
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 2*n if n % 3 != 2, else a(n) = ceil(n/2)
 * Range:     u8 n <= 68, u16 n <= 68, u32 n <= 68, u64 n <= 68, u128 n <= 68,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if ((n % 3) == 2)
        return ((seq_t)n + 1) / 2;
    else
        return ((seq_t)n) * 2;
}
