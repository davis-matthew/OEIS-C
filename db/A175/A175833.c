/*
 * A175833  Periodic sequence: repeat 4,7,11.
 * https://oeis.org/A175833
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-3) for n > 2, a(0)=4, a(1)=7, a(2)=11
 * Range:     u8 n <= 86, u16 n <= 86, u32 n <= 86, u64 n <= 86, u128 n <= 86,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    idx_t r = n % 3;
    if (r == 0) return (seq_t)4;
    if (r == 1) return (seq_t)7;
    /* r == 2 */
    return (seq_t)11;
}
