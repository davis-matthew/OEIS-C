/*
 * A107840  a(n) = 3*a(n-1) - 3*a(n-3) + a(n-4), n>6.
 * https://oeis.org/A107840
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*a(n-1) - 3*a(n-3) + a(n-4) for n>6 Initial values:
 *            a(1)=1, a(2)=1, a(3)=1, a(4)=0, a(5)=4, a(6)=8
 * Range:     u8 n <= 9, u16 n <= 15, u32 n <= 26, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* pre‑computed first six terms */
    static const seq_t base[7] = { 0, 1, 1, 1, 0, 4, 8 };

    if (n <= 6)
        return base[n];

    /* last five computed values: a(i-5) ... a(i-1)  */
    seq_t buf[5] = { base[2], base[3], base[4], base[5], base[6] }; /* a(2)..a(6) */

    for (idx_t i = 7; i <= n; ++i) {
        /* recurrence: a(i) = 3*a(i-1) - 3*a(i-3) + a(i-4) */
        seq_t new_val = 3 * buf[4] - 3 * buf[2] + buf[1];

        /* shift buffer left and store new value */
        buf[0] = buf[1];
        buf[1] = buf[2];
        buf[2] = buf[3];
        buf[3] = buf[4];
        buf[4] = new_val;
    }

    /* buf[4] now holds a(n) */
    return buf[4];
}
