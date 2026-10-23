/*
 * A156635  a(n) = 144*n^2 - n.
 * https://oeis.org/A156635
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 144*n^2 - n
 * Range:     u8 n <= 1, u16 n <= 21, u32 n <= 5461, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Convert the index to seq_t before any arithmetic to avoid
       accidental promotion to a smaller type. */
    seq_t nn = (seq_t)n;
    /* Because the recurrence is a simple quadratic, direct computation
       is O(1) and will be exact as long as the result fits within the
       underlying seq_t type. */
    return 144 * nn * nn - nn;
}
