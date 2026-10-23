/*
 * A008687  Number of 1's in 2's complement representation of -n.
 * https://oeis.org/A008687
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(0) = 0, a(1) = 1; a(n) = (n > 1) ? ((n & 1) + a((n & 1) + (n
 *            >> 1))) : n (0 ≤ n). This recurrence yields the number of 1's in
 *            the two's complement representation of –n
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    idx_t x   = n;   /* current value being transformed    */
    idx_t sum = 0;   /* accumulated number of 1's so far  */

    while (x > 1)
    {
        sum += x & 1;                     /* add low–bit  */
        x   = (x & 1) + (x >> 1);         /* recurse   */
    }

    return (seq_t)(sum + x);              /* sum + 0 or 1 for n ≤ 1 */
}
