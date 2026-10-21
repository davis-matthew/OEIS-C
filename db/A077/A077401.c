/*
 * A077401  Second member of Diophantine pair (m,k) that satisfies 7*(m^2 + m)
 *          = k^2 + k; a(n) = k.
 * https://oeis.org/A077401
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 16*a(n-2) - a(n-4) + 7 (for n ≥ 4); a(0) = 0, a(1) = 6,
 *            a(2) = 14, a(3) = 104
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 32, u128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Small indices are handled directly. */
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)6;
    if (n == 2) return (seq_t)14;
    if (n == 3) return (seq_t)104;

    /* Keep the last four terms needed for the recurrence. */
    seq_t t0 = (seq_t)0;   /* a(i-4)   for i = 4,5,...     */
    seq_t t1 = (seq_t)6;   /* a(i-3)   for i = 4,5,...     */
    seq_t t2 = (seq_t)14;  /* a(i-2)   for i = 4,5,...     */
    seq_t t3 = (seq_t)104; /* a(i-1)   for i = 4,5,...     */
    seq_t next;

    for (idx_t i = 4; i <= n; ++i)
    {
        /* a(i) = 16*a(i-2) - a(i-4) + 7 */
        next = (seq_t)16 * t2 - t0 + (seq_t)7;

        /* shift the sliding window for the next iteration */
        t0 = t1;
        t1 = t2;
        t2 = t3;
        t3 = next;
    }
    return t3;
}
