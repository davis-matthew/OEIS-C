/*
 * A165020  Length of cycle mentioned in A165019.
 * https://oeis.org/A165020
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = 2 for n <= 4; a(n) = 3 for n >= 5
 * Range:     u8 n <= 199, u16 n <= 199, u32 n <= 199, u64 n <= 199,
 *            u128 n <= 199, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n <= 4)
        return (seq_t)2;
    else
        return (seq_t)3;
}
