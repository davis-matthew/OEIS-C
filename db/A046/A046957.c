/*
 * A046957  Numbers k where cos(k) decreases monotonically to 0.
 * https://oeis.org/A046957
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 0 (n=1), 1 (n=2), 5 (n=3), 11 (n=4), 699 + 710*(n-5)
 *            (n>=5)
 * Range:     u8 n <= 4, u16 n <= 42, u32 n <= 42, u64 n <= 42, u128 n <= 42,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)0;
    if (n == 2) return (seq_t)1;
    if (n == 3) return (seq_t)5;
    if (n == 4) return (seq_t)11;
    /* n >= 5 */
    return (seq_t)699 + (seq_t)(n - 5) * (seq_t)710;
}
