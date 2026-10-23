/*
 * A015603  a(n) = 11*a(n-1) + 9*a(n-2).
 * https://oeis.org/A015603
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 11*a(n-1) + 9*a(n-2), a(0) = 0, a(1) = 1
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 10, u64 n <= 19, u128 n <= 37,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Iterate using the recurrence.  This method works for any
       seq_t or idx_t type, and requires only O(n) arithmetic
       operations.  No dynamic allocation or memoisation is needed. */
    seq_t a0 = (seq_t)0;
    seq_t a1 = (seq_t)1;

    if (n == 0) return a0;
    if (n == 1) return a1;

    seq_t an = (seq_t)0;
    for (idx_t i = 2; i <= n; ++i) {
        an = (seq_t)11 * a1 + (seq_t)9 * a0;
        a0 = a1;
        a1 = an;
    }
    return an;
}
