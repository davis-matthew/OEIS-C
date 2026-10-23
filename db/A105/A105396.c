/*
 * A105396  A simple "Fractal Jump Sequence" (FJS).
 * https://oeis.org/A105396
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = 3 if 3|n, otherwise 6
 * Range:     u8 n <= 107, u16 n <= 107, u32 n <= 107, u64 n <= 107,
 *            u128 n <= 107, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* For n < 3, behavior is irrelevant as the sequence is defined for n ≥ 3 */
    return (n % 3 == 0) ? (seq_t)3 : (seq_t)6;
}
