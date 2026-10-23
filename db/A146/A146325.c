/*
 * A146325  Period 3: repeat [1, 4, 1].
 * https://oeis.org/A146325
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n mod 3)^(n mod 3) with 0^0 = 1
 * Range:     u8 n <= 100, u16 n <= 100, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    idx_t m = n % 3;
    if (m == 2)
        return (seq_t)4;
    return (seq_t)1;
}
