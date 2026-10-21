/*
 * A228451  Recurrence: a(2n) = a(n), a(2n+1) = a(n) + 2n + 1, with a(0) = 0,
 *          a(1) = 1.
 * https://oeis.org/A228451
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, look
 * Formula:   a(0)=0, a(1)=1, a(2n) = a(n), a(2n+1) = a(n) + 2n + 1
 * Range:     u8 n <= 142, u16 n <= 8306, u32 n <= 8306, u64 n <= 8306,
 *            u128 n <= 8306, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)1;

    if ((n & 1ULL) == 0)  /* even:  n = 2k */
    {
        return a(n >> 1);
    }
    else                  /* odd:   n = 2k + 1 */
    {
        seq_t val = a(n >> 1);  /* a(k) */
        return val + (seq_t)n;  /* a(k) + (2k + 1) = a(n) */
    }
}
