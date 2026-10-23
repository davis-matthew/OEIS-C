/*
 * A166677  a(n)= n*(a(n-1)+4), a(0)=1.
 * https://oeis.org/A166677
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = n * (a(n-1) + 4), a(0) = 1
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 11, u64 n <= 19, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the base case */
    if (n == 0) {
        return (seq_t)1;
    }

    /* Iteratively apply the recurrence */
    seq_t val = (seq_t)1;          /* a(0) */
    for (idx_t i = 1; i <= n; ++i) {
        val = (seq_t)i * (val + (seq_t)4);
    }
    return val;
}
