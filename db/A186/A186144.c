/*
 * A186144  Number of elements in the symmetric group S_n whose distance from
 *          a fixed element is the group diameter under compositions of (1,2)
 *          and (1,2,...,n).
 * https://oeis.org/A186144
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, hard, more
 * Formula:   a(n) = 1 if n = 1 or 2 * a(n) = 3 if n = 3 or 4 * a(n) = 2 if n
 *            > 4 and n odd * a(n) = 1 if n > 4 and n even * This follows the
 *            OEIS conjecture for A186144
 * Range:     u8 n <= 13, u16 n <= 13, u32 n <= 13, u64 n <= 13, u128 n <= 13,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n <= 2) return (seq_t)1;
    if (n <= 4) return (seq_t)3;
    return (seq_t)((n & 1) ? 2 : 1);
}
