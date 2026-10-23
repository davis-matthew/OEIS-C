/*
 * A160572  Elements of A160444, pairs of consecutive entries swapped.
 * https://oeis.org/A160572
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(1)=1, a(2)=0, a(3)=1, a(4)=1; for n>=5: a(n)=2*a(n-2)+2*a(n-4)
 * Range:     u8 n <= 14, u16 n <= 24, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the offset: a(1) is the first term */
    if (n == 0) return 0;   /* undefined offset, but safe return */
    /* Base cases */
    if (n == 1) return 1;
    if (n == 2) return 0;
    if (n == 3) return 1;
    if (n == 4) return 1;

    /* Circular buffer holding a(n-4), a(n-3), a(n-2), a(n-1) */
    seq_t buf[4] = {1, 0, 1, 1};   /* a1, a2, a3, a4 */

    for (idx_t i = 5; i <= n; ++i) {
        /* a(i) = 2*a(i-2) + 2*a(i-4) */
        seq_t val = (buf[2] << 1) + (buf[0] << 1);
        /* Shift the buffer: drop oldest, shift others left, put new at the end */
        buf[0] = buf[1];
        buf[1] = buf[2];
        buf[2] = buf[3];
        buf[3] = val;
    }
    return buf[3];
}
