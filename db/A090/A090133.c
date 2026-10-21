/*
 * A090133  Expansion of (1+4x)/(1+4x+5x^2).
 * https://oeis.org/A090133
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign
 * Formula:   a(n) = -4*a(n-1) - 5*a(n-2), a(0)=1, a(1)=0 (from
 *            1+4x)/(1+4x+5x^2) expansion
 * Range:     i8 n <= 5, i16 n <= 12, i32 n <= 25, i64 n <= 29, i128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle first two terms explicitly */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)0;

    /* Iteratively compute up to n */
    seq_t prevprev = (seq_t)1;   /* a(0) */
    seq_t prev     = (seq_t)0;   /* a(1) */
    seq_t curr;
    idx_t i;

    for (i = 2; i <= n; ++i) {
        curr = -(seq_t)4 * prev - (seq_t)5 * prevprev;
        prevprev = prev;
        prev = curr;
    }
    return curr;
}
