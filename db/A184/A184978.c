/*
 * A184978  The months of the Gregorian calendar of less than 31 days.
 * https://oeis.org/A184978
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) = {2,4,6,9,11}[(n-1) mod 5]
 * Range:     u8 n <= 5, u16 n <= 5, u32 n <= 5, u64 n <= 5, u128 n <= 5,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Returns the n-th month number of the Gregorian calendar that has less than
   31 days.  The sequence repeats every five terms: 2, 4, 6, 9, 11. */
seq_t a(idx_t n)
{
    /* static const will be in read‑only memory */
    static const seq_t months[5] = {2, 4, 6, 9, 11};
    /* n>=1 by specification; modulo 5 yields 0..4 index */
    return months[(n - 1) % 5];
}
