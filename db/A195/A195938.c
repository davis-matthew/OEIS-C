/*
 * A195938  a(n) = n/2 if n mod 4 = 2, 0 otherwise.
 * https://oeis.org/A195938
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n/2 if n mod 4 = 2, 0 otherwise
 * Range:     u8 n <= 513, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if ((n & 3ul) == 2)
        return ((seq_t)n) / 2;
    return 0;
}
