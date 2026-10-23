/*
 * A133642  a(n) = 4*a(n-1)+20*a(n-2) for n>=3, a(0)=1, a(1)=4, a(2)=32.
 * https://oeis.org/A133642
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 4*a(n-1)+20*a(n-2); a(0)=1, a(1)=4, a(2)=32
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 11, u64 n <= 21, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 1;
    if (n == 1) return 4;
    if (n == 2) return 32;

    seq_t a_prev2 = 1;          /* a(0)   */
    seq_t a_prev1 = 4;          /* a(1)   */
    seq_t a_curr  = 32;         /* a(2)   */

    for (idx_t i = 3; i <= n; ++i) {
        seq_t next = (seq_t)4 * a_curr + (seq_t)20 * a_prev1;
        a_prev1 = a_curr;
        a_curr  = next;
    }
    return a_curr;
}
