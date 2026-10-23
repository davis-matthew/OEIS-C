/*
 * A019515  a(n) = 1 + 0!*1!*2!*...*n!.
 * https://oeis.org/A019515
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1 + 0!*1!*2!*...*n!
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 6, u64 n <= 8, u128 n <= 11,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t prod = 1;      /* product of factorials up to the current i */
    seq_t fact = 1;      /* current factorial i! (starting with 0! = 1) */

    for (idx_t i = 0; i <= n; ++i) {
        prod *= fact;          /* multiply into the overall product */
        if (i < n) {
            fact *= (seq_t)(i + 1);   /* advance factorial to (i+1)! */
        }
    }
    return prod + 1;            /* final sequence value */
}
