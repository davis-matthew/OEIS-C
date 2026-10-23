/*
 * A038521  Number of elements of GF(2^n) with trace 1 and subtrace 1.
 * https://oeis.org/A038521
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 2*a(n-2) + 4*a(n-3) for n>3 with a(0)=0, a(1)=0, a(2)=2,
 *            a(3)=1
 * Range:     u8 n <= 9, u16 n <= 17, u32 n <= 33, u64 n <= 65, u128 n <= 129,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the four base cases first. */
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)0;
    if (n == 2) return (seq_t)2;
    if (n == 3) return (seq_t)1;

    /* A small fixed‑size ring buffer keeps the last four values
       needed for the recurrence. */
    seq_t buf[4] = { (seq_t)0, (seq_t)0, (seq_t)2, (seq_t)1 };

    for (idx_t i = 4; i <= n; ++i)
    {
        /* Compute a(i) = 2*a(i-2) + 4*a(i-3) using indices modulo 4. */
        idx_t i2 = (i - 2) & 3u;
        idx_t i3 = (i - 3) & 3u;
        seq_t next = (seq_t)2 * buf[i2] + (seq_t)4 * buf[i3];
        buf[i & 3u] = next;
    }
    return buf[n & 3u];
}
