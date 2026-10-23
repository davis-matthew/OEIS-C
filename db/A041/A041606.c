/*
 * A041606  Numerators of continued fraction convergents to sqrt(321).
 * https://oeis.org/A041606
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 13, u64 n <= 27, u128 n <= 57,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A041606: Numerators of convergents to sqrt(321)
   Recurrence:
        p_{-2} = 0,   p_{-1} = 1
        p_n   = a_n * p_{n-1} + p_{n-2}
   where a_0 = 17, and for n ≥ 1, a_n repeats [1, 10, 1, 34] */
seq_t a(idx_t n) {
    static const seq_t pattern[4] = {1, 10, 1, 34};   /* a_n for n ≥ 1 */
    seq_t prev2 = 0;   /* p_{-2} */
    seq_t prev1 = 1;   /* p_{-1} */
    seq_t cur, a_i;

    for (idx_t i = 0; i <= n; ++i) {
        if (i == 0)
            a_i = 17;
        else
            a_i = pattern[(i - 1) % 4];

        cur = a_i * prev1 + prev2;

        if (i == n)
            return cur;

        prev2 = prev1;
        prev1 = cur;
    }
    /* Should never reach here */
    return 0;
}
