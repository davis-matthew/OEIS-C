/*
 * A187093  a(0)=0, a(1)=a(2)=1; thereafter, a(n+1) = n^2 - a(n-1).
 * https://oeis.org/A187093
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=0, a(1)=a(2)=1; a(n+1)=n^2 - a(n-1) for n>=2
 * Range:     u8 n <= 22, u16 n <= 362, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;
    if (n <= 2)
        return 1;

    seq_t a_prev2 = 1; /* a(1) */
    seq_t a_prev1 = 1; /* a(2) */
    seq_t a_next;

    for (idx_t i = 2; i < n; ++i) {
        a_next = (seq_t)i * (seq_t)i - a_prev2; /* a(i+1) */
        a_prev2 = a_prev1;
        a_prev1 = a_next;
    }
    return a_prev1; /* a(n) */
}
