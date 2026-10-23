/*
 * A011943  Numbers k such that any group of k consecutive integers has
 *          integral standard deviation (viz. A011944(k)).
 * https://oeis.org/A011943
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 14*a(n-1) - a(n-2); a(1) = 1, a(2) = 7
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 18, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)7;

    seq_t prev = (seq_t)1;          /* a(n-2)  */
    seq_t curr = (seq_t)7;          /* a(n-1)  */

    for (idx_t i = 3; i <= n; ++i)
    {
        seq_t next = (seq_t)14 * curr - prev;  /* a(n) = 14*a(n-1) - a(n-2) */
        prev = curr;
        curr = next;
    }
    return curr;
}
