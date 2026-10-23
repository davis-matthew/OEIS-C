/*
 * A122701  a(0)=0, a(n) = 2*a(floor(n/2)) + n - 1 for n > 0.
 * https://oeis.org/A122701
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*a(floor(n/2)) + n - 1, a(0) = 0, a(1) = 0
 * Range:     u8 n <= 61, u16 n <= 61, u32 n <= 61, u64 n <= 61, u128 n <= 61,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0U)
        return (seq_t)0;
    if (n == 1U)
        return (seq_t)0;

    return (seq_t)(2 * a(n / 2) + (seq_t)(n - 1));
}
