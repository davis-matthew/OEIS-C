/*
 * A028139  Expansion of 1/((1-4x)(1-6x)(1-9x)(1-10x)).
 * https://oeis.org/A028139
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (5*10^(n+3)-8*9^(n+3)+5*6^(n+3)-2*4^(n+3))/120
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 8, u64 n <= 16, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Initial values a(0) .. a(3) */
    static const seq_t a0 = (seq_t)1;
    static const seq_t a1 = (seq_t)29;
    static const seq_t a2 = (seq_t)537;
    static const seq_t a3 = (seq_t)8113;

    if (n == 0) return a0;
    if (n == 1) return a1;
    if (n == 2) return a2;
    if (n == 3) return a3;

    /* Coefficients from the characteristic polynomial */
    const seq_t e1 = (seq_t)29;
    const seq_t e2 = (seq_t)304;
    const seq_t e3 = (seq_t)1356;
    const seq_t e4 = (seq_t)2160;

    seq_t last[4] = { a0, a1, a2, a3 };
    seq_t next;

    for (idx_t i = 4; i <= n; ++i) {
        /* a(i) = e1*a(i-1) - e2*a(i-2) + e3*a(i-3) - e4*a(i-4) */
        seq_t part1 = e1 * last[3] + e3 * last[1];
        seq_t part2 = e2 * last[2] + e4 * last[0];
        next = part1 - part2;

        /* shift the window */
        last[0] = last[1];
        last[1] = last[2];
        last[2] = last[3];
        last[3] = next;
    }

    return last[3];
}
