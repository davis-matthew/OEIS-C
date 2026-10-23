/*
 * A179742  The number of syllables in each letter of the English alphabet.
 * https://oeis.org/A179742
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  fini, nonn, word, full
 * Formula:   a(n) = 3 if n ≡ 23 (mod 26) otherwise 1
 * Range:     u8 n <= 26, u16 n <= 26, u32 n <= 26, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    /* zero‑based index of the letter */
    idx_t i = (n - 1) % 26;
    /* W is the 23rd letter (index 22) */
    return i == 22 ? (seq_t)3 : (seq_t)1;
}
