/*
 * A209229  Characteristic function of powers of 2, cf. A000079.
 * https://oeis.org/A209229
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, mult, easy
 * Formula:   a(n) = 1 if n is a power of 2; 0 otherwise (a(0)=0)
 * Range:     u8 n <= 9659, u16 n <= 9659, u32 n <= 9659, u64 n <= 9659,
 *            u128 n <= 9659, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;
    return (seq_t)(((n & (n - 1)) == 0) ? 1 : 0);
}
