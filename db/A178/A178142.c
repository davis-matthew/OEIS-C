/*
 * A178142  Sum over the divisors d = 2 and/or 3 of n.
 * https://oeis.org/A178142
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, less
 * Formula:   a(n) = Sum_{d|n, d=2 or d=3} d (periodic with period
 *            {0,2,3,2,0,5})
 * Range:     u8 n <= 105, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* The sequence repeats every 6 with values:
       n mod 6: 0 -> 5, 1 -> 0, 2 -> 2, 3 -> 3, 4 -> 2, 5 -> 0          */
    static const seq_t period[6] = {5, 0, 2, 3, 2, 0};
    return period[n % 6];
}
