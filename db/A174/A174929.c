/*
 * A174929  Partial sums of A174928.
 * https://oeis.org/A174929
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (71 + 264*n + 130*n^2 - 63*(-1)^n) / 8. Initial values:
 *            a(0)=1, a(1)=66, a(2)=132
 * Range:     u8 n <= 0, u16 n <= 21, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    seq_t val = 130 * nn * nn + 264 * nn + 71;
    if ((n & 1U) == 0) {      /* n even: (-1)^n = 1, subtract 63 */
        val -= 63;
    } else {                  /* n odd:  (-1)^n = -1, subtract -63 -> add 63 */
        val += 63;
    }
    return val / 8;
}
