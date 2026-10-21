/*
 * A087778  Decimal expansion of experimental Avogadro's constant (in
 *          mol^(-1)).
 * https://oeis.org/A087778
 *
 * Offset:    24    (a(24) is the first term)
 * Keywords:  dead
 * Formula:   a(n) = the n'th decimal digit of 602214179000000000000000 (for n
 *            ≥ 24). The value has 24 non‑zero digits followed by arbitrarily
 *            many zeros
 * Range:     i8 n <= 30, i16 n <= 30, i32 n <= 30, i64 n <= 30, i128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 24) {
        /* The sequence is defined only for n ≥ 24.  Return 0 for safety. */
        return 0;
    }

    /* 24 known digits of the experimental Avogadro constant. */
    static const seq_t digits[24] = {
        6, 0, 2, 2, 1, 4, 1, 7, 9,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
    };

    idx_t pos = n - 24;
    return (pos < 24) ? digits[pos] : 0;
}
