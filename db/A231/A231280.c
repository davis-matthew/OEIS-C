/*
 * A231280  Number of n X 3 0..3 arrays x(i,j) with each element horizontally
 *          or antidiagonally next to at least one element with value
 *          (x(i,j)+1) mod 4, and upper left element zero.
 * https://oeis.org/A231280
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 4*a(n-1) + 17*a(n-2) (a(1)=0, a(2)=2)
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 13, u64 n <= 25, u128 n <= 48,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* For n < 1 the sequence is undefined; return 0. */
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)0;
    if (n == 2) return (seq_t)2;

    /* Iterate the linear recurrence.  Only the two most recent values
       need to be kept, so the memory usage is O(1). */
    seq_t prev2 = (seq_t)0;   /* a(1) */
    seq_t prev1 = (seq_t)2;   /* a(2) */
    seq_t current;

    for (idx_t i = 3; i <= n; ++i) {
        current = (seq_t)(4 * prev1 + 17 * prev2);
        prev2 = prev1;
        prev1 = current;
    }
    return prev1;
}
