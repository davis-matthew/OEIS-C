/*
 * A206545  Period length 16: repeat 1, 3, 5, 7, 9, 11, 13, 15, 15, 13, 11, 9,
 *          7, 5, 3, 1.
 * https://oeis.org/A206545
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*n-1 for n ≡ 1..8 (mod 16); a(n) = 2*(16-(n mod 16)+1)-1
 *            for n ≡ 9..0 (mod 16)
 * Range:     u8 n <= 80, u16 n <= 80, u32 n <= 80, u64 n <= 80, u128 n <= 80,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    /* reduce n to the [1,16] range using the period 16 */
    idx_t r = (n - 1) % 16 + 1;          /* r ∈ [1,16] */

    if (r <= 8) {
        /* for 1..8: a(n) = 2*n-1 */
        return (seq_t)(2 * r - 1);
    } else {
        /* for 9..16: a(n) = 2*(17-r)-1  (symmetric) */
        return (seq_t)(2 * (17 - r) - 1);
    }
}
