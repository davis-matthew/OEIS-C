/*
 * A156638  Numbers k such that k^2 + 2 == 0 (mod 9).
 * https://oeis.org/A156638
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 9*((n-1)/2) + (n%2?4:5) (valid for n >= 1)
 * Range:     u8 n <= 56, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    idx_t blocks = (n - 1) / 2;          /* number of full 2‑term cycles before n   */
    seq_t value = (seq_t)blocks * (seq_t)9; /* 9 * cycles                                   */
    if (n & 1) {                         /* odd n → add 4, even n → add 5             */
        value += (seq_t)4;
    } else {
        value += (seq_t)5;
    }
    return value;
}
