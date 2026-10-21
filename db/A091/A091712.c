/*
 * A091712  a(n) = 6*(2n-4)!/((n-2)!n!), if n>2. a(0) = 1, a(1) = a(2) = 2.
 * https://oeis.org/A091712
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 6*(2n-4)!/((n-2)!n!) for n>2, a(0)=1, a(1)=a(2)=2.
 *            Recurrence: a(n) = a(n-1)*(4n-10)/n for n>3
 * Range:     u8 n <= 8, u16 n <= 13, u32 n <= 20, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* handle the pre‑defined small values */
    if (n == 0)
        return (seq_t)1;
    if (n <= 3)
        return (seq_t)2;

    /* start from a(3) = 2 and apply the recurrence up to n */
    seq_t result = (seq_t)2;
    for (idx_t k = 4; k <= n; ++k)
    {
        result = result * (seq_t)(4 * k - 10) / (seq_t)k;
    }
    return result;
}
