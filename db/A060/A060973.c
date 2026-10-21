/*
 * A060973  a(2*n+1) = a(n+1)+a(n), a(2*n) = 2*a(n), with a(1)=0 and a(2)=1.
 * https://oeis.org/A060973
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(1)=0, a(2)=1; * a(2*n) = 2*a(n), for n ≥ 1 * a(2*n+1) =
 *            a(n+1)+a(n), for n ≥ 0 * offset: 1 * This implementation uses a
 *            simple recursion that follows the * recurrence directly. The
 *            depth of the recursion is O(log n), so * it is fast for the
 *            required range and requires no mutable static * state....
 * Range:     u8 n <= 511, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static seq_t a_recursive(idx_t n)
{
    if (n == 1)                     /* base case: a(1)=0 */
        return (seq_t)0;
    if (n == 2)                     /* base case: a(2)=1 */
        return (seq_t)1;
    if ((n & 1u) == 0)              /* even index: a(2*n)=2*a(n) */
        return (seq_t)2 * a_recursive(n >> 1);
    /* odd index: a(2*n+1)=a(n+1)+a(n) */
    idx_t k = n >> 1;               /* k = ⌊n/2⌋ */
    return a_recursive(k) + a_recursive(k + 1);
}

seq_t a(idx_t n)
{
    return a_recursive(n);
}
