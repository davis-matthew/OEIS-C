/*
 * A069325  Number of 3 X n binary arrays with path of adjacent 1's from upper
 *          right corner to lower left corner.
 * https://oeis.org/A069325
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(1) = 1, a(2) = 8, a(3) = 51, a(4) = 295, a(5) = 1632, a(6) =
 *            8830, a(7) = 47239, a(n) = 9*a(n-1) - 21*a(n-2) + 3*a(n-3) +
 *            24*a(n-4) - 8*a(n-5) - 4*a(n-6) + 4*a(n-7) for n ≥ 8
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 13, u64 n <= 27, u128 n <= 53,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* initial values (index 0 unused) */
    static const seq_t init[8] = {
        (seq_t)0,
        (seq_t)1,   /* a(1) */
        (seq_t)8,   /* a(2) */
        (seq_t)51,  /* a(3) */
        (seq_t)295, /* a(4) */
        (seq_t)1632,/* a(5) */
        (seq_t)8830,/* a(6) */
        (seq_t)47239/* a(7) */
    };

    if (n <= 7)
        return init[n];

    /* last 7 values: a(n-7) .. a(n-1) */
    seq_t buf[7];
    buf[0] = init[1];
    buf[1] = init[2];
    buf[2] = init[3];
    buf[3] = init[4];
    buf[4] = init[5];
    buf[5] = init[6];
    buf[6] = init[7];

    for (idx_t i = 8; i <= n; ++i) {
        /* pos = 9*a(n-1) + 3*a(n-3) + 24*a(n-4) + 4*a(n-7)   */
        seq_t pos = 9 * buf[6];
        pos += 3 * buf[4];
        pos += 24 * buf[3];
        pos += 4 * buf[0];

        /* neg = 21*a(n-2) + 8*a(n-5) + 4*a(n-6)               */
        seq_t neg = 21 * buf[5];
        neg += 8 * buf[2];
        neg += 4 * buf[1];

        seq_t next = pos - neg;   /* recurrence */

        /* shift buffer: a(n-6) -> a(n-7) ... a(n) -> a(n-1) */
        buf[0] = buf[1];
        buf[1] = buf[2];
        buf[2] = buf[3];
        buf[3] = buf[4];
        buf[4] = buf[5];
        buf[5] = buf[6];
        buf[6] = next;
    }

    return buf[6];
}
