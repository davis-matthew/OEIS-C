/*
 * A193622  Integers originally used as card values in Planning Poker.
 * https://oeis.org/A193622
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) = 1 + (n/2)^2 if n is even a(n) = ((n-1)/2)^2 + ((n-1)/2) +
 *            1 if n is odd where division is integer division truncating
 *            toward zero
 * Range:     u8 n <= 6, u16 n <= 6, u32 n <= 6, u64 n <= 6, u128 n <= 6,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)              /* undefined for the sequence, return 0 */
        return (seq_t)0;

    if ((n & (idx_t)1) == 0) /* n is even */
    {
        idx_t half = n >> 1;
        seq_t h = (seq_t)half;
        return (seq_t)1 + h * h;
    }
    else                    /* n is odd */
    {
        idx_t half = (n - (idx_t)1) >> 1;
        seq_t h = (seq_t)half;
        return h * h + h + (seq_t)1;
    }
}
