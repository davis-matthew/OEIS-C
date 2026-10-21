/*
 * A093140  Expansion of (1-6*x)/((1-x)*(1-10*x)).
 * https://oeis.org/A093140
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 4*10^n/9 + 5/9, recurrence: a(0) = 1, a(n) = 10*a(n-1) -
 *            5
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 9, u64 n <= 19, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t val = 1;          /* a(0) */
    while (n > 0) {
        val = val * 10 - 5; /* a(n) = 10*a(n-1) - 5 */
        n--;
    }
    return val;
}
