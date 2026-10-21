/*
 * A216985  Number of city-block distance 1, pressure limit 2 movements in an
 *          n X 2 array with each element moving exactly one horizontally or
 *          vertically, no element acquiring more than two neighbors, and
 *          without 2-loops.
 * https://oeis.org/A216985
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 7*a(n-1) - 9*a(n-2) + a(n-3); a(1)=0, a(2)=2, a(3)=14
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 14, u64 n <= 27, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle base cases explicitly.  The recurrence is valid only for n ≥ 4. */
    if (n == 1) return 0;
    if (n == 2) return 2;
    if (n == 3) return 14;

    /* Sliding window for the recurrence.  a1 = a(n-3), a2 = a(n-2), a3 = a(n-1). */
    seq_t a1 = 0;
    seq_t a2 = 2;
    seq_t a3 = 14;

    for (idx_t i = 4; i <= n; ++i)
    {
        seq_t next = 7 * a3 - 9 * a2 + a1;  /* a(n) = 7*a(n-1) - 9*a(n-2) + a(n-3) */
        a1 = a2;
        a2 = a3;
        a3 = next;
    }
    return a3;
}
