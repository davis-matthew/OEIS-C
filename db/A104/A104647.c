/*
 * A104647  a(n) = a(n-1) mod n + a(n-2) mod n; a(0) = 0, a(1) = 1.
 * https://oeis.org/A104647
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) mod n + a(n-2) mod n; a(0)=0, a(1)=1
 * Range:     u8 n <= 194, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)1;
    seq_t a_prev2 = (seq_t)0;  /* a(0) */
    seq_t a_prev  = (seq_t)1;  /* a(1) */
    seq_t a_cur = 0;
    for (idx_t i = 2; i <= n; i++)
    {
        a_cur = (a_prev % (seq_t)i) + (a_prev2 % (seq_t)i);
        a_prev2 = a_prev;
        a_prev  = a_cur;
    }
    return a_cur;
}
