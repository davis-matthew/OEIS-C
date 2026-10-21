/*
 * A084053  4*n digit-reversed mod 4.
 * https://oeis.org/A084053
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = rev(4*n) mod 4
 * Range:     u8 n <= 64, u16 n <= 107, u32 n <= 107, u64 n <= 107,
 *            u128 n <= 107, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t x = (seq_t)n * 4;
    int revmod = 0;
    while (x != 0) {
        seq_t d = x % 10;          /* last decimal digit of x */
        revmod = (revmod * 10 + (int)d) % 4;  /* update reversed number modulo 4 */
        x = x / 10;                /* remove last digit */
    }
    return (seq_t)revmod;
}
