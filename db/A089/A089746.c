/*
 * A089746  Period 12: repeat [4, 4, 1, 2, 1, 1, 2, 2, 3, 3, 3, 3] (number of
 *          syllables in English name of the months).
 * https://oeis.org/A089746
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, word
 * Formula:   a(n) = period[(n-1) mod 12] where period = [4, 4, 1, 2, 1, 1, 2,
 *            2, 3, 3, 3, 3] (syllables in month names)
 * Range:     u8 n <= 96, u16 n <= 96, u32 n <= 96, u64 n <= 96, u128 n <= 96,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* 12-period array of syllable counts for the English month names */
    static const seq_t period[12] = {4, 4, 1, 2, 1, 1, 2, 2, 3, 3, 3, 3};
    /* n is 1-based; (n-1) % 12 gives the correct index in the period array */
    idx_t idx = (n - 1) % 12;
    return period[idx];
}
