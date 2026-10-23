/*
 * A047521  Numbers that are congruent to {0, 7} mod 8.
 * https://oeis.org/A047521
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4*n − (n%2==0 ? 1 : 4) (a(1)=0, a(2)=7, …)
 * Range:     u8 n <= 64, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t val = 4 * (seq_t)n;
    if ((n & 1) == 0)          /* even n  → n%2==0 */
        val -= 1;              /* subtract 1 for even indices */
    else
        val -= 4;              /* subtract 4 for odd indices   */
    return val;
}
