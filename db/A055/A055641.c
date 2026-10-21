/*
 * A055641  Number of zero digits in n.
 * https://oeis.org/A055641
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = number of zero digits in decimal representation of n
 *            (a(0) = 1, a(1..9) = 0)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t cnt = 0;          /* result accumulator */
    if (n == 0)             /* special case for n = 0 */
        return (seq_t)1;

    idx_t x = n;            /* work on a copy of n */
    while (x > 0) {
        if ((x % 10) == 0)
            cnt++;
        x /= 10;
    }
    return cnt;
}
