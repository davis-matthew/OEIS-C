/*
 * A056207  Number of binary trees of height <= n.
 * https://oeis.org/A056207
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = a(n-1)^2 + 4*a(n-1) + 3 with a(1) = 3
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 4, u64 n <= 5, u128 n <= 6,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle indices below the offset (not required, but safe) */
    if (n < 1)
        return 0;

    seq_t val = 3;             /* a(1) */
    if (n == 1)
        return val;

    /* Iterate using the recurrence.  All arithmetic is performed on
       seq_t, which may be a basic integer type or an arbitrary-precision
       big integer class, so the operations are defined for the chosen type. */
    for (idx_t i = 2; i <= n; ++i) {
        seq_t next = val * val;    /* val^2 */
        next += 4 * val;           /* + 4*val */
        next += 3;                 /* + 3 */
        val = next;
    }
    return val;
}
