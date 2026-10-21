/*
 * A083039  Number of divisors of n that are <= 3.
 * https://oeis.org/A083039
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 1 + [2|n] + [3|n] (number of divisors of n that are ≤ 3)
 * Range:     u8 n <= 9474, u16 n <= 9474, u32 n <= 9474, u64 n <= 9474,
 *            u128 n <= 9474, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t res = 1;                /* divisor 1 is always present */
    if ((n % 2) == 0)             /* divisor 2 */
        res++;
    if ((n % 3) == 0)             /* divisor 3 */
        res++;
    return res;
}
