/*
 * A011906  If b(n) is A011900(n) and c(n) is A001109(n), then a(n) =
 *          b(n)*c(n) = b(n) + (b(n)+1) + (b(n)+2) + ... + c(n).
 * https://oeis.org/A011906
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 41*a(n-1) - 246*a(n-2) + 246*a(n-3) - 41*a(n-4) + a(n-5)
 *            initial values: a(1)=1, a(2)=18, a(3)=525, a(4)=17340,
 *            a(5)=586177
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 13, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;               /* no definition for n=0 */

    /* first 5 known terms */
    static const seq_t init[5] = {1, 18, 525, 17340, 586177};

    if (n <= 5) return init[n-1];

    /* buffer holding the last 5 terms: a(n-5) ... a(n-1) */
    seq_t buf[5] = {init[0], init[1], init[2], init[3], init[4]};

    /* iterate from term 6 up to the requested term */
    for (idx_t i = 6; i <= n; ++i) {
        seq_t next = 41 * buf[4]
                   - 246 * buf[3]
                   + 246 * buf[2]
                   - 41  * buf[1]
                   +      buf[0];

        /* shift the window: drop the oldest term and append the new one */
        buf[0] = buf[1];
        buf[1] = buf[2];
        buf[2] = buf[3];
        buf[3] = buf[4];
        buf[4] = next;
    }

    return buf[4];   /* buf[4] now holds a(n) */
}
