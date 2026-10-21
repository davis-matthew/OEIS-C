/*
 * A064867  The minimal number which has multiplicative persistence 3 in base
 *          n.
 * https://oeis.org/A064867
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = 4*n - floor(n/6) for n > 5 with the initial values
 *            a(3)=26, a(4)=63, a(5)=68
 * Range:     u8 n <= 66, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 3) return 26;
    if (n == 4) return 63;
    if (n == 5) return 68;
    /* n >= 6: use the closed‐form formula */
    seq_t val = (seq_t)n * (seq_t)4;
    val -= (seq_t)(n / 6);   /* floor division on idx_t */
    return val;
}
