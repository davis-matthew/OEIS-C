/*
 * A052386  Number of integers from 1 to 10^n-1 that lack 0 as a digit.
 * https://oeis.org/A052386
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, base
 * Formula:   a(n) = 9*a(n-1) + 9, a(0) = 0
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 10, u64 n <= 20, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the number of integers from 1 to 10^n-1 that lack 0 as a digit. */
seq_t a(idx_t n)
{
    seq_t res = 0;
    for (idx_t i = 1; i <= n; ++i) {
        res = res * 9 + 9;      /* recurrence: a(i) = 9*a(i-1) + 9 */
    }
    return res;
}
