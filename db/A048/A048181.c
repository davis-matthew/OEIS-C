/*
 * A048181  Chisel strokes in Runes numerals (= alphabet characters).
 * https://oeis.org/A048181
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  fini, full, nonn
 * Formula:   not stated
 * Range:     u8 n <= 24, u16 n <= 24, u32 n <= 24, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The number of strokes needed to write the rune that represents the
   numeric value n in the Elder Futhark runic numerals.
   For n > 24 there is no corresponding rune, so a(n) = 0. */
seq_t a(idx_t n)
{
    /* Pre‑computed stroke counts for runes 1 .. 24. */
    static const seq_t table[24] = {
        3, 3, 3, 3, 4, 2, 2, 3,
        3, 2, 1, 2, 1, 5, 3, 3,
        3, 5, 4, 4, 2, 4, 4, 4
    };

    if (n == 0 || n > 24)
        return (seq_t)0;
    return table[n - 1];
}
