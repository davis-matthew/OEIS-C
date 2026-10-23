/*
 * A176067  Binary analog of A098755 and A098756: Binary representation of
 *          least available nonnegative integer with bit values alternating
 *          for entire sequence.
 * https://oeis.org/A176067
 *
 * Offset:    -1    (a(-1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = 101*a(n-1) - 100*a(n-2), a(0)=0, a(1)=10, a(-1)=1
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 10, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == -1) return (seq_t)1;
    if (n == 0)  return (seq_t)0;
    if (n == 1)  return (seq_t)10;

    seq_t prev2 = (seq_t)0;
    seq_t prev1 = (seq_t)10;
    seq_t cur;

    for (idx_t i = 2; i <= n; ++i) {
        cur   = prev1 * (seq_t)101 - prev2 * (seq_t)100;
        prev2 = prev1;
        prev1 = cur;
    }

    return prev1;
}
