/*
 * A083040  Number of divisors of n that are <= 4.
 * https://oeis.org/A083040
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 1 + (n mod 2 == 0) + (n mod 3 == 0) + (n mod 4 == 0)
 * Range:     u8 n <= 72, u16 n <= 72, u32 n <= 72, u64 n <= 72, u128 n <= 72,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Number of divisors of n that are <= 4. */
seq_t a(idx_t n)
{
    seq_t count = 1;          /* divisor 1 always divides n */
    if ((n % 2) == 0) count++;      /* divisor 2 */
    if ((n % 3) == 0) count++;      /* divisor 3 */
    if ((n % 4) == 0) count++;      /* divisor 4 */
    return count;
}
