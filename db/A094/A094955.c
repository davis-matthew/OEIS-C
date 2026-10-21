/*
 * A094955  Main diagonal of array A094954.
 * https://oeis.org/A094955
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = y(n+1,n+1) where y(m+1,n) = n*y(m,n)-y(m-1,n) with
 *            y(0,n)=y(1,n)=y(2,n)=1 for all n. The offset is 1, hence a(1)=1
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 16, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* k = n+1 is the coefficient used in the recurrence that
       generates the main‐diagonal element. */
    idx_t k = n + 1;

    /* For k = 1 or 2 the value is 1 (this covers n = 0, which is
       outside the required offset, and n = 1). */
    if (k <= 2)
        return 1;

    /* Initial values y(0), y(1), y(2) = 1 for any n.  We only need
       the two most recent values for the recurrence. */
    seq_t prev2 = 1;   /* y(m-2)  -> y(1) */
    seq_t prev1 = 1;   /* y(m-1)  -> y(2) */
    seq_t cur   = 1;   /* placeholder for y(m) */

    /* Iterate m = 3 .. k computing y(m) = k * y(m-1) - y(m-2). */
    for (idx_t m = 3; m <= k; ++m) {
        cur   = (seq_t)k * prev1 - prev2;
        prev2 = prev1;
        prev1 = cur;
    }

    return cur;   /* cur now holds y(k) = a(n) */
}
