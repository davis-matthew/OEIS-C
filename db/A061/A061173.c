/*
 * A061173  One-fourth of fourth column of Lucas bisection triangle (odd
 *          part).
 * https://oeis.org/A061173
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 13, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A061173: a(n) = A060924(n+3,3)/4.
 * Recurrence (n ≥ 8):
 *   a(n) = 12*a(n-1) - 58*a(n-2) + 144*a(n-3)
 *            - 195*a(n-4) + 144*a(n-5) - 58*a(n-6)
 *            + 12*a(n-7) - a(n-8)
 * Initial values:
 *   a(0)=3, a(1)=70, a(2)=642, a(3)=4055,
 *   a(4)=20945, a(5)=95372, a(6)=398290, a(7)=1561683. */

seq_t a(idx_t n)
{
    /* first 8 terms */
    const seq_t init[8] = {
        (seq_t)3, (seq_t)70, (seq_t)642, (seq_t)4055,
        (seq_t)20945, (seq_t)95372, (seq_t)398290, (seq_t)1561683
    };

    if (n < 8)
        return init[n];

    seq_t buf[8];
    for (int i = 0; i < 8; ++i)
        buf[i] = init[i];

    for (idx_t i = 8; i <= n; ++i) {
        seq_t val = 12 * buf[7] + 144 * buf[5] + 144 * buf[3] + 12 * buf[1];
        val -= 58 * buf[6] + 195 * buf[4] + 58 * buf[2] + buf[0];

        /* shift buffer left, drop oldest term */
        for (int k = 0; k < 7; ++k)
            buf[k] = buf[k + 1];
        buf[7] = val;
    }

    return buf[7];
}
