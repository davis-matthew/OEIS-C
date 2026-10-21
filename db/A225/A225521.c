/*
 * A225521  Cumulative number of letters in first n English names of playing
 *          card denominations: ace, two, three, ... jack, queen, king.
 * https://oeis.org/A225521
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, word, fini, full, less
 * Formula:   a(n) = ∑_{i=1}^{n} length_of_card_name(i) (card names repeat
 *            cyclically)
 * Range:     u8 n <= 13, u16 n <= 13, u32 n <= 13, u64 n <= 13, u128 n <= 13,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* letter counts for the 13 card denominations */
    static const unsigned char card_lengths[13] = {
        3, /* ace   */
        3, /* two   */
        5, /* three */
        4, /* four  */
        4, /* five  */
        3, /* six   */
        5, /* seven */
        5, /* eight */
        4, /* nine  */
        3, /* ten   */
        4, /* jack  */
        5, /* queen */
        4  /* king  */
    };

    const seq_t full_cycle = 52;          /* total letters in one full cycle */

    idx_t cycles = n / 13;                /* full cycles up to n */
    idx_t rem    = n % 13;                /* remaining (partial) cycle */

    seq_t sum = (seq_t)cycles * full_cycle;

    for (idx_t i = 0; i < rem; ++i) {
        sum += (seq_t)card_lengths[i];
    }

    return sum;
}
