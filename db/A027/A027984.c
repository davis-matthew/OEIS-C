/*
 * A027984  a(n) = Sum_{k=0..n} T(n, k)*T(n, n+k), T given by A027960.
 * https://oeis.org/A027984
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 4*a(n-1) - 3*a(n-2) - 2*a(n-3) + a(n-4), with a(0)=1,
 *            a(1)=6, a(2)=20, a(3)=58
 * Range:     u8 n <= 4, u16 n <= 10, u32 n <= 21, u64 n <= 44, u128 n <= 90,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the first four values directly. */
    if (n == 0u) return (seq_t)1;
    if (n == 1u) return (seq_t)6;
    if (n == 2u) return (seq_t)20;
    if (n == 3u) return (seq_t)58;

    /* Keep only the last four terms of the recurrence. */
    seq_t buf[4];
    buf[0] = (seq_t)1;
    buf[1] = (seq_t)6;
    buf[2] = (seq_t)20;
    buf[3] = (seq_t)58;

    for (idx_t i = 4u; i <= n; ++i) {
        seq_t next = 4 * buf[3] - 3 * buf[2] - 2 * buf[1] + buf[0];
        buf[0] = buf[1];
        buf[1] = buf[2];
        buf[2] = buf[3];
        buf[3] = next;
    }
    return buf[3];
}
