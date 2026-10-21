/*
 * A234429  Numbers which are the digital sum of the square of some prime.
 * https://oeis.org/A234429
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = 4 (n=1), 7 (n=2), 9 (n=3), 3*(n-1)+1 (n≥4)
 * Range:     u8 n <= 60, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)4;
    if (n == 2) return (seq_t)7;
    if (n == 3) return (seq_t)9;
    /* n ≥ 4 */
    return 3 * (seq_t)(n - 1) + (seq_t)1;
}
