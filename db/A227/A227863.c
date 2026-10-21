/*
 * A227863  Numbers congruent to {1,49} mod 120.
 * https://oeis.org/A227863
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 60*n - 6*(-1)^n - 65
 * Range:     u8 n <= 5, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* offset is 1; behaviour for n == 0 is undefined but we handle it safely. */
    if (n == 0)
        return (seq_t)0;

    seq_t res = (seq_t)60 * (seq_t)n;
    if ((n & 1) == 0)      /* n is even: (-1)^n = 1  -> subtract 71 */
        res -= (seq_t)71;
    else                   /* n is odd:  (-1)^n = -1 -> subtract 59 */
        res -= (seq_t)59;
    return res;
}
