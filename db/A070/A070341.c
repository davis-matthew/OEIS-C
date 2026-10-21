/*
 * A070341  a(n) = 3^n mod 11: Repeat (1, 3, 9, 5, 4), period 5.
 * https://oeis.org/A070341
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3^n mod 11
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    static const seq_t table[5] = { (seq_t)1, (seq_t)3, (seq_t)9, (seq_t)5, (seq_t)4 };
    return table[n % 5];
}
