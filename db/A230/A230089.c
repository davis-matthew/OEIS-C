/*
 * A230089  If n is divisible by 4 then 4, if n is divisible by 2 then 2,
 *          otherwise n.
 * https://oeis.org/A230089
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4 if n%4==0, 2 if n%2==0, else n
 * Range:     u8 n <= 90, u16 n <= 90, u32 n <= 90, u64 n <= 90, u128 n <= 90,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n % 4 == 0)
        return (seq_t)4;
    if (n % 2 == 0)
        return (seq_t)2;
    return (seq_t)n;
}
