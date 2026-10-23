/*
 * A042292  Numerators of continued fraction convergents to sqrt(672).
 * https://oeis.org/A042292
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   a(n) = 674*a(n-4) - a(n-8) for n >= 8 with initial values:
 *            a(0)=25, a(1)=26, a(2)=311, a(3)=337, a(4)=17161, a(5)=17498,
 *            a(6)=209639, a(7)=227137
 * Range:     u8 n <= 1, u16 n <= 5, u32 n <= 11, u64 n <= 25, u128 n <= 53,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    static const seq_t init[8] = {25, 26, 311, 337,
                                  17161, 17498, 209639, 227137};

    if (n < 8)
        return init[n];

    seq_t buf[8];
    for (idx_t i = 0; i < 8; ++i)
        buf[i] = init[i];

    for (idx_t i = 8; i <= n; ++i) {
        seq_t val = ((seq_t)674) * buf[(i - 4) & 7] - buf[(i - 8) & 7];
        buf[i & 7] = val;
    }

    return buf[n & 7];
}
