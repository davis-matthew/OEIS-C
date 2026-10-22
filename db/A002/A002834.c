/*
 * A002834  Number of ways of getting a straight flush, 4 of a kind, full
 *          house, flush, straight, 3 of a kind, 2 pair, a pair, nothing in a
 *          3-card poker hand.
 * https://oeis.org/A002834
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  fini, nonn, full
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 9, u32 n <= 9, u64 n <= 9, u128 n <= 9,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: 3-card poker hand classification counts (52-card deck, 12 non-wrapping straights)
 * a(1) = 12 * 4 (straight flush)
 * a(2) = 0 (4 of a kind, impossible)
 * a(3) = 0 (full house, impossible in 3-card)
 * a(4) = C(13,3)*4 - 48 (flush without straight)
 * a(5) = 12 * 60 (straight without flush)
 * a(6) = 13 * 4 (3 of a kind)
 * a(7) = 0 (2 pair, impossible)
 * a(8) = 13 * C(4,2) * 12 * 4 (pair)
 * a(9) = C(13,3)*64 - 12*64 - (C(13,3)*4 - 48) (nothing)
 */

seq_t a(idx_t n) {
    if (n == 1) {
        return (seq_t)(12 * 4);
    }
    if (n == 2 || n == 3 || n == 7) {
        return (seq_t)0;
    }
    if (n == 4) {
        seq_t c13_3 = (seq_t)(13 * 12 * 11 / 6);
        return c13_3 * 4 - 48;
    }
    if (n == 5) {
        return (seq_t)(12 * 60);
    }
    if (n == 6) {
        return (seq_t)(13 * 4);
    }
    if (n == 8) {
        return (seq_t)(13 * 6 * 12 * 4);
    }
    if (n == 9) {
        seq_t c13_3 = (seq_t)(13 * 12 * 11 / 6);
        seq_t all_diff = c13_3 * 64;
        seq_t straights = (seq_t)(12 * 64);
        seq_t flushes = c13_3 * 4 - 48;
        return all_diff - straights - flushes;
    }
    return (seq_t)0;
}
