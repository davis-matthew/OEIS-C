/*
 * A187295  Number of 10-step one or two space at a time rook's tours on an n
 *          X n board summed over all starting positions.
 * https://oeis.org/A187295
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 29244672*n^2 - 280021012*n + 623972304 for n>17. * For n
 *            ≤ 17 the values are 0, 0, 0, 789696, 10664384, 48391584, *
 *            132995984, 276318288, 482853584, 752575536, 1083942480, *
 *            1475455440, 1926184124, 2435663832, 3003713808, 3630272848, *
 *            4315325376
 * Range:     u8 n <= 3, u16 n <= 3, u32 n <= 16, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    static const seq_t small[17] = {
        (seq_t)0, (seq_t)0, (seq_t)0, (seq_t)789696,
        (seq_t)10664384, (seq_t)48391584, (seq_t)132995984,
        (seq_t)276318288, (seq_t)482853584, (seq_t)752575536,
        (seq_t)1083942480, (seq_t)1475455440, (seq_t)1926184124,
        (seq_t)2435663832, (seq_t)3003713808, (seq_t)3630272848,
        (seq_t)4315325376
    };
    if (n <= 17)
        return small[n - 1];

    seq_t m = (seq_t)n;
    seq_t res = (seq_t)29244672 * m * m;
    res -= (seq_t)280021012 * m;
    res += (seq_t)623972304;
    return res;
}
