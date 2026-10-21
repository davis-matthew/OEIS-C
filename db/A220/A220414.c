/*
 * A220414  a(n) = 6*a(n-1) - a(n-2), with a(1) = 13, a(2) = 73.
 * https://oeis.org/A220414
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 6*a(n-1) - a(n-2), a(1)=13, a(2)=73
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 12, u64 n <= 24, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the two known initial terms */
    if (n == 1)
        return (seq_t)13;
    if (n == 2)
        return (seq_t)73;

    /* Compute the sequence by iterating the recurrence.  */
    seq_t prev = (seq_t)13;           /* a(1) */
    seq_t curr = (seq_t)73;           /* a(2) */
    seq_t next;

    for (idx_t i = 3; i <= n; ++i) {
        next  = (seq_t)6 * curr - prev;
        prev  = curr;
        curr  = next;
    }
    return curr;
}
