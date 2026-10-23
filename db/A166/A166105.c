/*
 * A166105  Quadratic recurrence from Sylvester's sequence, but starting with
 *          a(0)=1 and a(1)=2.
 * https://oeis.org/A166105
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1; a(n+1) = a(n)*(a(n)-1) + 2
 * Range:     u8 n <= 4, u16 n <= 5, u32 n <= 6, u64 n <= 7, u128 n <= 8,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t res = 1;
    for (idx_t i = 1; i <= n; ++i)
    {
        res = res * (res - 1) + 2;
    }
    return res;
}
