/*
 * A116454  Smallest m such that A116452(m) = n.
 * https://oeis.org/A116454
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(1) = 0, a(2) = 2, for n > 1: a(n+1) = (a(n) + n) * 2 + 1
 * Range:     u8 n <= 6, u16 n <= 14, u32 n <= 30, u64 n <= 33, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n <= 1) return (seq_t)0;          /* a(1) */
    if (n == 2) return (seq_t)2;           /* a(2) */
    seq_t a_curr = (seq_t)2;               /* a(2) */
    for (idx_t i = 2; i < n; ++i) {        /* compute a(i+1) until a(n) */
        seq_t next = ((a_curr + (seq_t)i) * (seq_t)2) + (seq_t)1;
        a_curr    = next;
    }
    return a_curr;
}
