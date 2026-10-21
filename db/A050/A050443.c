/*
 * A050443  a(0)=4, a(1)=0, a(2)=0, a(3)=3; thereafter a(n) = a(n-3) + a(n-4).
 * https://oeis.org/A050443
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(0)=4, a(1)=0, a(2)=0, a(3)=3; thereafter a(n) = a(n-3) +
 *            a(n-4)
 * Range:     u8 n <= 27, u16 n <= 55, u32 n <= 111, u64 n <= 222,
 *            u128 n <= 444, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the initial values directly. */
    if (n == 0) return 4;
    if (n == 1) return 0;
    if (n == 2) return 0;
    if (n == 3) return 3;

    /* Buffer for the last four computed terms. */
    seq_t buf[4] = {4, 0, 0, 3};

    /* Compute terms from 4 up to n using the recurrence. */
    for (idx_t i = 4; i <= n; ++i) {
        seq_t val = buf[(i - 3) & 3] + buf[(i - 4) & 3];
        buf[i & 3] = val;
    }

    return buf[n & 3];
}
