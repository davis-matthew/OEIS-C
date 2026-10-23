/*
 * A136784  Number of primitive multiplex juggling sequences of length n, base
 *          state <3> and hand capacity 3.
 * https://oeis.org/A136784
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 14, u64 n <= 27, u128 n <= 52,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = 9*a(n-1) - 21*a(n-2) + 13*a(n-3)
 * Initial values: a(1) = 1, a(2) = 3, a(3) = 13
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;          /* undefined for 0, but guard anyway */

    if (n == 1)
        return (seq_t)1;
    if (n == 2)
        return (seq_t)3;
    if (n == 3)
        return (seq_t)13;

    seq_t a1 = (seq_t)1;
    seq_t a2 = (seq_t)3;
    seq_t a3 = (seq_t)13;
    seq_t ai;

    for (idx_t i = 4; i <= n; ++i) {
        ai = (seq_t)9 * a3 - (seq_t)21 * a2 + (seq_t)13 * a1;
        a1 = a2;
        a2 = a3;
        a3 = ai;
    }

    return a3;
}
