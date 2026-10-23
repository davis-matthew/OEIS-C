/*
 * A117412  Sum of the interior angles of an n-sided polygon, in gradians.
 * https://oeis.org/A117412
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 200*(n-2)
 * Range:     u8 n <= 3, u16 n <= 329, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 3) return 0;
    return (seq_t)200 * ((seq_t)n - 2);
}
