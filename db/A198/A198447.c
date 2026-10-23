/*
 * A198447  Number of 2n X 2 0..2 arrays with values 0..2 introduced in row
 *          major order and each element unequal to exactly two horizontal and
 *          vertical neighbors.
 * https://oeis.org/A198447
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 10*a(n-1) - 25*a(n-2) + 16*a(n-3) Initial values: a(1)=3,
 *            a(2)=13, a(3)=71 (valid for all n >= 1)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 24, u128 n <= 47,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the offset and trivial cases */
    if (n == 0)
        return (seq_t)0;          /* defined only for n >= 1 */

    if (n == 1)
        return (seq_t)3;
    if (n == 2)
        return (seq_t)13;
    if (n == 3)
        return (seq_t)71;

    /* Use three variables to iterate the recurrence */
    seq_t prev3 = (seq_t)3;   /* a(n-3) */
    seq_t prev2 = (seq_t)13;  /* a(n-2) */
    seq_t prev1 = (seq_t)71;  /* a(n-1) */

    seq_t cur;
    for (idx_t i = 4; i <= n; ++i) {
        cur = prev1 * (seq_t)10;   /* 10*a(n-1) */
        cur -= prev2 * (seq_t)25;  /* -25*a(n-2) */
        cur += prev3 * (seq_t)16;  /* +16*a(n-3) */

        /* Shift for next iteration */
        prev3 = prev2;
        prev2 = prev1;
        prev1 = cur;
    }
    return prev1;
}
