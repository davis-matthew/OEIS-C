/*
 * A200141  Upper bound by J. Rivat and J. Wu on constant arising in
 *          Piatetski-Shapiro primes.
 * https://oeis.org/A200141
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, cons
 * Formula:   a(n) = 1 for n = 1,2; for n >= 3 the sequence is periodic with
 *            period 5 and values [8,5,3,6,5], i.e. a(n) = pattern[(n-3) mod
 *            5]
 * Range:     u8 n <= 99, u16 n <= 99, u32 n <= 99, u64 n <= 99, u128 n <= 99,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n <= 2)           /* a(1) = a(2) = 1 */
        return (seq_t)1;
    /* period 5 pattern starting at n = 3 */
    static const seq_t pattern[5] = {8, 5, 3, 6, 5};
    return pattern[(n - 3) % 5];
}
